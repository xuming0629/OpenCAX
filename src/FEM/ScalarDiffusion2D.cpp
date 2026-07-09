#include <OpenCAX/FEM/ScalarDiffusion2D.h>
#include <OpenCAX/FEM/TriangleP1Element.h>

#include <cmath>
#include <iostream>
#include <vector>

namespace OpenCAX
{

ScalarDiffusion2D::ScalarDiffusion2D(const P1TriangleSpace& space)
    : PhysicsProblem2D(space)
{
    source_ = [](double, double) { return 0.0; };
}

void ScalarDiffusion2D::setCoefficient(double k)
{
    materials_.setDefaultConstant(k);
}

void ScalarDiffusion2D::setCoefficient(ScalarFunction2D k)
{
    materials_.setDefaultCoefficient(std::move(k));
}

void ScalarDiffusion2D::setSource(Function2D f)
{
    source_ = std::move(f);
}

void ScalarDiffusion2D::addDirichletBC(BoundarySelector2D selector, Function2D value)
{
    boundaries_.addDirichlet(std::move(selector), std::move(value));
}

void ScalarDiffusion2D::addNeumannBC(BoundarySelector2D selector, Function2D value)
{
    boundaries_.addNeumann(std::move(selector), std::move(value));
}

void ScalarDiffusion2D::assemble()
{
    const std::size_t n = space_.numDofs();
    const std::size_t nc = space_.numCells();

    system_.resize(n);
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(nc * 9);

    for (std::size_t cid = 0; cid < nc; ++cid)
    {
        assembleCell(cid, triplets);
    }

    system_.matrix().setFromTriplets(triplets.begin(), triplets.end());
    system_.matrix().makeCompressed();

    applyNeumann();
}

void ScalarDiffusion2D::assembleCell(std::size_t cell_id, std::vector<Eigen::Triplet<double>>& triplets)
{
    const auto& mesh = space_.mesh();
    const auto& cells = mesh.cells();
    const auto& nodes = mesh.nodes();
    const auto& cell = cells[cell_id];
    const auto dofs = space_.cellDofs(cell_id);

    if (dofs.size() != 3) return;

    const MeshNode& p0 = nodes[dofs[0]];
    const MeshNode& p1 = nodes[dofs[1]];
    const MeshNode& p2 = nodes[dofs[2]];

    TriangleP1Geometry geo;
    if (!TriangleP1Element::computeGeometry(p0, p1, p2, geo)) return;

    const double x = geo.centroid[0];
    const double y = geo.centroid[1];
    const double k = materials_.scalarValue(cell, x, y);
    const double f = source_(x, y);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            const double kij = k * geo.area * (
                geo.dphidx[i] * geo.dphidx[j] + geo.dphidy[i] * geo.dphidy[j]);
            triplets.emplace_back(dofs[i], dofs[j], kij);
        }
        system_.rhs()(dofs[i]) += f * geo.area / 3.0;
    }
}

void ScalarDiffusion2D::applyNeumann()
{
    const auto& bcs = boundaries_.neumann();
    if (bcs.empty()) return;

    const auto& topo = space_.topology();
    const auto& edges = topo.edges();
    const auto& boundary_edges = topo.boundary_edges();
    const auto& nodes = space_.mesh().nodes();

    for (int eid : boundary_edges)
    {
        const auto& e = edges[static_cast<std::size_t>(eid)];
        const int a = e[0];
        const int b = e[1];
        const auto& n0 = nodes[static_cast<std::size_t>(a)];
        const auto& n1 = nodes[static_cast<std::size_t>(b)];

        const double mx = 0.5 * (n0.x + n1.x);
        const double my = 0.5 * (n0.y + n1.y);
        const double ds = std::sqrt((n1.x - n0.x) * (n1.x - n0.x) + (n1.y - n0.y) * (n1.y - n0.y));

        for (const auto& bc : bcs)
        {
            if (!bc.matchEdge(n0, n1)) continue;
            const double q = bc.value(mx, my);
            system_.rhs()(a) += 0.5 * q * ds;
            system_.rhs()(b) += 0.5 * q * ds;
        }
    }
}

void ScalarDiffusion2D::applyBoundaryConditions()
{
    applyDirichletSparse();
}

void ScalarDiffusion2D::applyDirichletSparse()
{
    const int n = static_cast<int>(system_.size());
    if (n == 0) return;

    const auto& nodes = space_.mesh().nodes();
    const auto boundary_dofs = space_.boundaryDofs();

    std::vector<char> is_d(n, 0);
    std::vector<double> val(n, 0.0);

    for (int dof : boundary_dofs)
    {
        if (dof < 0 || dof >= n) continue;
        double g = 0.0;
        if (boundaries_.dirichletValue(nodes[static_cast<std::size_t>(dof)], g))
        {
            is_d[dof] = 1;
            val[dof] = g;
        }
    }

    const auto& K = system_.matrix();
    auto& F = system_.rhs();
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(static_cast<std::size_t>(K.nonZeros()) + boundary_dofs.size());

    for (int col = 0; col < K.outerSize(); ++col)
    {
        for (Eigen::SparseMatrix<double>::InnerIterator it(K, col); it; ++it)
        {
            const int row = it.row();
            const int c = it.col();
            const double a = it.value();
            const bool rd = is_d[row] != 0;
            const bool cd = is_d[c] != 0;

            if (!rd && cd) F(row) -= a * val[c];
            if (!rd && !cd) triplets.emplace_back(row, c, a);
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (!is_d[i]) continue;
        triplets.emplace_back(i, i, 1.0);
        F(i) = val[i];
    }

    Eigen::SparseMatrix<double> Knew(n, n);
    Knew.setFromTriplets(triplets.begin(), triplets.end());
    Knew.makeCompressed();
    system_.matrix() = std::move(Knew);
}

} // namespace OpenCAX

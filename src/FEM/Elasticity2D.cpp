#include <OpenCAX/FEM/Elasticity2D.h>
#include <OpenCAX/FEM/SparseLUSolver.h>
#include <OpenCAX/FEM/TriangleP1Element.h>

#include <cmath>
#include <iostream>
#include <vector>

namespace OpenCAX
{

Elasticity2D::Elasticity2D(const P1TriangleSpace& space)
    : space_(space)
{
    body_force_ = [](double, double) { return Eigen::Vector2d::Zero(); };
    solver_ = std::make_shared<SparseLUSolver>();
}

void Elasticity2D::setMaterial(const LinearElasticMaterial2D& material) { material_ = material; }
void Elasticity2D::setBodyForce(VectorFunction2D body_force) { body_force_ = std::move(body_force); }
void Elasticity2D::addDirichletBC(BoundarySelector2D selector, VectorFunction2D displacement) { dirichlet_bcs_.push_back({std::move(selector), std::move(displacement)}); }
void Elasticity2D::addTractionBC(BoundarySelector2D selector, VectorFunction2D traction) { traction_bcs_.push_back({std::move(selector), std::move(traction)}); }
void Elasticity2D::setSolver(std::shared_ptr<LinearSolver> solver) { solver_ = std::move(solver); }
const Eigen::VectorXd& Elasticity2D::displacement() const { return system_.solution(); }
LinearSystem& Elasticity2D::linearSystem() { return system_; }
const LinearSystem& Elasticity2D::linearSystem() const { return system_; }

int Elasticity2D::uxDof(int node_id) const { return 2 * node_id; }
int Elasticity2D::uyDof(int node_id) const { return 2 * node_id + 1; }

Eigen::Vector2d Elasticity2D::nodeDisplacement(int node_id) const
{
    return Eigen::Vector2d(system_.solution()(uxDof(node_id)), system_.solution()(uyDof(node_id)));
}

void Elasticity2D::assemble()
{
    const std::size_t ndof = 2 * space_.numDofs();
    system_.resize(ndof);

    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(space_.numCells() * 36);

    for (std::size_t cid = 0; cid < space_.numCells(); ++cid)
    {
        assembleCell(cid, triplets);
    }

    system_.matrix().setFromTriplets(triplets.begin(), triplets.end());
    system_.matrix().makeCompressed();

    applyTractionBC();
}

void Elasticity2D::assembleCell(std::size_t cell_id, std::vector<Eigen::Triplet<double>>& triplets)
{
    const auto& nodes = space_.mesh().nodes();
    const auto dofs = space_.cellDofs(cell_id);
    if (dofs.size() != 3) return;

    TriangleP1Geometry geo;
    if (!TriangleP1Element::computeGeometry(nodes[dofs[0]], nodes[dofs[1]], nodes[dofs[2]], geo)) return;

    Eigen::Matrix<double, 3, 6> B;
    B.setZero();

    for (int i = 0; i < 3; ++i)
    {
        B(0, 2 * i)     = geo.dphidx[i];
        B(1, 2 * i + 1) = geo.dphidy[i];
        B(2, 2 * i)     = geo.dphidy[i];
        B(2, 2 * i + 1) = geo.dphidx[i];
    }

    const Eigen::Matrix3d D = material_.constitutiveMatrix();
    const Eigen::Matrix<double, 6, 6> Ke = geo.area * (B.transpose() * D * B);

    int gdof[6] = {
        uxDof(dofs[0]), uyDof(dofs[0]),
        uxDof(dofs[1]), uyDof(dofs[1]),
        uxDof(dofs[2]), uyDof(dofs[2])
    };

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            triplets.emplace_back(gdof[i], gdof[j], Ke(i, j));
        }
    }

    const Eigen::Vector2d bf = body_force_(geo.centroid[0], geo.centroid[1]);
    for (int i = 0; i < 3; ++i)
    {
        system_.rhs()(uxDof(dofs[i])) += bf.x() * geo.area / 3.0;
        system_.rhs()(uyDof(dofs[i])) += bf.y() * geo.area / 3.0;
    }
}

void Elasticity2D::applyTractionBC()
{
    if (traction_bcs_.empty()) return;

    const auto& topo = space_.topology();
    const auto& edges = topo.edges();
    const auto& bedges = topo.boundary_edges();
    const auto& nodes = space_.mesh().nodes();

    for (int eid : bedges)
    {
        const auto& e = edges[static_cast<std::size_t>(eid)];
        const int a = e[0];
        const int b = e[1];
        const auto& n0 = nodes[static_cast<std::size_t>(a)];
        const auto& n1 = nodes[static_cast<std::size_t>(b)];
        const double mx = 0.5 * (n0.x + n1.x);
        const double my = 0.5 * (n0.y + n1.y);
        const double ds = std::sqrt((n1.x - n0.x) * (n1.x - n0.x) + (n1.y - n0.y) * (n1.y - n0.y));

        for (const auto& bc : traction_bcs_)
        {
            if (!bc.selector.matchEdge(n0, n1)) continue;
            Eigen::Vector2d t = bc.value(mx, my);
            system_.rhs()(uxDof(a)) += 0.5 * t.x() * ds;
            system_.rhs()(uyDof(a)) += 0.5 * t.y() * ds;
            system_.rhs()(uxDof(b)) += 0.5 * t.x() * ds;
            system_.rhs()(uyDof(b)) += 0.5 * t.y() * ds;
        }
    }
}

void Elasticity2D::applyBoundaryConditions()
{
    applyDirichletSparse();
}

void Elasticity2D::applyDirichletSparse()
{
    const int n = static_cast<int>(system_.size());
    const auto& nodes = space_.mesh().nodes();
    const auto bdofs = space_.boundaryDofs();

    std::vector<char> is_d(n, 0);
    std::vector<double> val(n, 0.0);

    for (int node_id : bdofs)
    {
        const auto& node = nodes[static_cast<std::size_t>(node_id)];
        for (const auto& bc : dirichlet_bcs_)
        {
            if (!bc.selector.matchPoint(node)) continue;
            Eigen::Vector2d u = bc.value(node.x, node.y);
            is_d[uxDof(node_id)] = 1;
            is_d[uyDof(node_id)] = 1;
            val[uxDof(node_id)] = u.x();
            val[uyDof(node_id)] = u.y();
        }
    }

    const auto& K = system_.matrix();
    auto& F = system_.rhs();
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(static_cast<std::size_t>(K.nonZeros()) + n);

    for (int col = 0; col < K.outerSize(); ++col)
    {
        for (Eigen::SparseMatrix<double>::InnerIterator it(K, col); it; ++it)
        {
            const int r = it.row();
            const int c = it.col();
            const double a = it.value();
            const bool rd = is_d[r] != 0;
            const bool cd = is_d[c] != 0;
            if (!rd && cd) F(r) -= a * val[c];
            if (!rd && !cd) triplets.emplace_back(r, c, a);
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

bool Elasticity2D::solve()
{
    assemble();
    applyBoundaryConditions();
    if (!solver_) return false;
    return solver_->solve(system_);
}

} // namespace OpenCAX

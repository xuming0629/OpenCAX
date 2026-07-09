#include <OpenCAX/FEM/HeatEquation2D.h>
#include <OpenCAX/FEM/TriangleP1Element.h>

#include <cmath>
#include <iostream>
#include <vector>

namespace OpenCAX
{

HeatEquation2D::HeatEquation2D(const P1TriangleSpace& space)
    : PhysicsProblem2D(space)
{
    source_ = [](double, double) { return 0.0; };
    initial_ = [](double, double) { return 0.0; };
}

void HeatEquation2D::setCapacity(double rho_cp) { capacity_ = rho_cp; }
void HeatEquation2D::setConductivity(double k) { materials_.setDefaultConstant(k); }
void HeatEquation2D::setConductivity(ScalarFunction2D k) { materials_.setDefaultCoefficient(std::move(k)); }
void HeatEquation2D::setSource(Function2D source) { source_ = std::move(source); }
void HeatEquation2D::setInitialCondition(Function2D initial) { initial_ = std::move(initial); }
void HeatEquation2D::addDirichletBC(BoundarySelector2D selector, Function2D value) { boundaries_.addDirichlet(std::move(selector), std::move(value)); }
void HeatEquation2D::addNeumannBC(BoundarySelector2D selector, Function2D value) { boundaries_.addNeumann(std::move(selector), std::move(value)); }
double HeatEquation2D::time() const { return time_; }

void HeatEquation2D::initialize()
{
    const auto& nodes = space_.mesh().nodes();
    const int n = static_cast<int>(nodes.size());
    U_old_ = Eigen::VectorXd::Zero(n);
    for (int i = 0; i < n; ++i)
    {
        U_old_(i) = initial_(nodes[static_cast<std::size_t>(i)].x, nodes[static_cast<std::size_t>(i)].y);
    }
    system_.resize(static_cast<std::size_t>(n));
    system_.solution() = U_old_;
    time_ = 0.0;
}

void HeatEquation2D::assemble()
{
    assembleMassAndStiffness();
}

void HeatEquation2D::applyBoundaryConditions()
{
}

void HeatEquation2D::assembleMassAndStiffness()
{
    const int n = static_cast<int>(space_.numDofs());
    M_.resize(n, n);
    K_.resize(n, n);
    F_ = Eigen::VectorXd::Zero(n);

    std::vector<Eigen::Triplet<double>> mtrip;
    std::vector<Eigen::Triplet<double>> ktrip;
    mtrip.reserve(space_.numCells() * 9);
    ktrip.reserve(space_.numCells() * 9);

    const auto& mesh = space_.mesh();
    const auto& nodes = mesh.nodes();
    const auto& cells = mesh.cells();

    for (std::size_t cid = 0; cid < space_.numCells(); ++cid)
    {
        auto dofs = space_.cellDofs(cid);
        if (dofs.size() != 3) continue;

        const auto& cell = cells[cid];
        TriangleP1Geometry geo;
        if (!TriangleP1Element::computeGeometry(nodes[dofs[0]], nodes[dofs[1]], nodes[dofs[2]], geo)) continue;

        const double x = geo.centroid[0];
        const double y = geo.centroid[1];
        const double k = materials_.scalarValue(cell, x, y);
        const double f = source_(x, y);

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                const double mij = capacity_ * geo.area * ((i == j) ? 2.0 : 1.0) / 12.0;
                const double kij = k * geo.area * (geo.dphidx[i] * geo.dphidx[j] + geo.dphidy[i] * geo.dphidy[j]);
                mtrip.emplace_back(dofs[i], dofs[j], mij);
                ktrip.emplace_back(dofs[i], dofs[j], kij);
            }
            F_(dofs[i]) += f * geo.area / 3.0;
        }
    }

    M_.setFromTriplets(mtrip.begin(), mtrip.end());
    K_.setFromTriplets(ktrip.begin(), ktrip.end());
    M_.makeCompressed();
    K_.makeCompressed();
}

void HeatEquation2D::assembleNeumann(Eigen::VectorXd& rhs) const
{
    const auto& bcs = boundaries_.neumann();
    if (bcs.empty()) return;

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

        for (const auto& bc : bcs)
        {
            if (!bc.matchEdge(n0, n1)) continue;
            const double q = bc.value(mx, my);
            rhs(a) += 0.5 * q * ds;
            rhs(b) += 0.5 * q * ds;
        }
    }
}

void HeatEquation2D::applyDirichletToSystem(Eigen::SparseMatrix<double>& A, Eigen::VectorXd& rhs)
{
    const int n = static_cast<int>(rhs.size());
    const auto& nodes = space_.mesh().nodes();
    const auto bdofs = space_.boundaryDofs();

    std::vector<char> is_d(n, 0);
    std::vector<double> val(n, 0.0);
    for (int dof : bdofs)
    {
        double g = 0.0;
        if (boundaries_.dirichletValue(nodes[static_cast<std::size_t>(dof)], g))
        {
            is_d[dof] = 1;
            val[dof] = g;
        }
    }

    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(static_cast<std::size_t>(A.nonZeros()) + bdofs.size());

    for (int col = 0; col < A.outerSize(); ++col)
    {
        for (Eigen::SparseMatrix<double>::InnerIterator it(A, col); it; ++it)
        {
            const int r = it.row();
            const int c = it.col();
            const double a = it.value();
            const bool rd = is_d[r] != 0;
            const bool cd = is_d[c] != 0;
            if (!rd && cd) rhs(r) -= a * val[c];
            if (!rd && !cd) triplets.emplace_back(r, c, a);
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (!is_d[i]) continue;
        triplets.emplace_back(i, i, 1.0);
        rhs(i) = val[i];
    }

    Eigen::SparseMatrix<double> Anew(n, n);
    Anew.setFromTriplets(triplets.begin(), triplets.end());
    Anew.makeCompressed();
    A = std::move(Anew);
}

bool HeatEquation2D::step(double dt)
{
    if (U_old_.size() == 0) initialize();

    assembleMassAndStiffness();

    Eigen::SparseMatrix<double> A = (1.0 / dt) * M_ + K_;
    Eigen::VectorXd rhs = (1.0 / dt) * (M_ * U_old_) + F_;
    assembleNeumann(rhs);
    applyDirichletToSystem(A, rhs);

    system_.resize(static_cast<std::size_t>(rhs.size()));
    system_.matrix() = A;
    system_.rhs() = rhs;

    if (!solver_) return false;
    if (!solver_->solve(system_)) return false;

    U_old_ = system_.solution();
    time_ += dt;
    return true;
}

bool HeatEquation2D::run(double dt, int steps)
{
    if (U_old_.size() == 0) initialize();
    for (int i = 0; i < steps; ++i)
    {
        if (!step(dt)) return false;
    }
    return true;
}

} // namespace OpenCAX

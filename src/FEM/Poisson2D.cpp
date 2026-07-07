#include <OpenCAX/FEM/Poisson2D.h>
#include <OpenCAX/FEM/SparseLUSolver.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace OpenCAX
{

Poisson2D::Poisson2D(
    const TriangleMesh& mesh
)
{
    owned_space_ =
        std::make_unique<P1TriangleSpace>(mesh);

    space_ = owned_space_.get();

    source_ = [](double, double) {
        return 1.0;
    };

    dirichlet_ = BoundaryCondition2D(
        BoundaryType::Dirichlet,
        [](double, double) {
            return 0.0;
        }
    );

    solver_ =
        std::make_shared<SparseLUSolver>();
}

Poisson2D::Poisson2D(
    const TriangleMesh& mesh,
    const MeshTopology& topology
)
{
    owned_space_ =
        std::make_unique<P1TriangleSpace>(
            mesh,
            topology
        );

    space_ = owned_space_.get();

    source_ = [](double, double) {
        return 1.0;
    };

    dirichlet_ = BoundaryCondition2D(
        BoundaryType::Dirichlet,
        [](double, double) {
            return 0.0;
        }
    );

    solver_ =
        std::make_shared<SparseLUSolver>();
}

Poisson2D::Poisson2D(
    const P1TriangleSpace& space
)
    : space_(&space)
{
    source_ = [](double, double) {
        return 1.0;
    };

    dirichlet_ = BoundaryCondition2D(
        BoundaryType::Dirichlet,
        [](double, double) {
            return 0.0;
        }
    );

    solver_ =
        std::make_shared<SparseLUSolver>();
}

void Poisson2D::setSource(
    Function2D f
)
{
    source_ = std::move(f);
}

void Poisson2D::setDirichlet(
    Function2D g
)
{
    dirichlet_ = BoundaryCondition2D(
        BoundaryType::Dirichlet,
        std::move(g)
    );
}

void Poisson2D::setSolver(
    std::shared_ptr<LinearSolver> solver
)
{
    solver_ = std::move(solver);
}

const P1TriangleSpace& Poisson2D::space() const
{
    return *space_;
}

LinearSystem& Poisson2D::linearSystem()
{
    return system_;
}

const LinearSystem& Poisson2D::linearSystem() const
{
    return system_;
}

const Eigen::VectorXd& Poisson2D::solution() const
{
    return system_.solution();
}

void Poisson2D::assemble()
{
    if (space_ == nullptr)
    {
        std::cerr << "[OpenCAX::Poisson2D] function space is null."
                  << std::endl;
        return;
    }

    assembleP1();
}

void Poisson2D::assembleP1()
{
    const std::size_t dof_count =
        space_->numDofs();

    const std::size_t cell_count =
        space_->numCells();

    system_.resize(dof_count);

    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(cell_count * 9);

    for (std::size_t cid = 0; cid < cell_count; ++cid)
    {
        assembleP1Cell(
            cid,
            triplets
        );
    }

    system_.matrix().setFromTriplets(
        triplets.begin(),
        triplets.end()
    );

    system_.matrix().makeCompressed();
}

void Poisson2D::assembleP1Cell(
    std::size_t cell_id,
    std::vector<Eigen::Triplet<double>>& triplets
)
{
    const std::vector<int> dofs =
        space_->cellDofs(cell_id);

    if (dofs.size() != 3)
    {
        std::cerr << "[OpenCAX::Poisson2D] invalid local dof count."
                  << std::endl;
        return;
    }

    const int id0 = dofs[0];
    const int id1 = dofs[1];
    const int id2 = dofs[2];

    const auto p0 = space_->dofPoint(id0);
    const auto p1 = space_->dofPoint(id1);
    const auto p2 = space_->dofPoint(id2);

    const double x0 = p0[0];
    const double y0 = p0[1];

    const double x1 = p1[0];
    const double y1 = p1[1];

    const double x2 = p2[0];
    const double y2 = p2[1];

    const double detJ =
        (x1 - x0) * (y2 - y0) -
        (x2 - x0) * (y1 - y0);

    const double area =
        0.5 * std::abs(detJ);

    if (area < 1.0e-14)
    {
        std::cerr << "[OpenCAX::Poisson2D] skip degenerated triangle: "
                  << cell_id
                  << std::endl;
        return;
    }

    const double twoA = 2.0 * area;

    double b[3];
    double c[3];

    b[0] = (y1 - y2) / twoA;
    b[1] = (y2 - y0) / twoA;
    b[2] = (y0 - y1) / twoA;

    c[0] = (x2 - x1) / twoA;
    c[1] = (x0 - x2) / twoA;
    c[2] = (x1 - x0) / twoA;

    const int ids[3] = {
        id0,
        id1,
        id2
    };

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            const double kij =
                area *
                (
                    b[i] * b[j] +
                    c[i] * c[j]
                );

            triplets.emplace_back(
                ids[i],
                ids[j],
                kij
            );
        }
    }

    const double xc =
        (x0 + x1 + x2) / 3.0;

    const double yc =
        (y0 + y1 + y2) / 3.0;

    const double fv =
        source_(xc, yc);

    for (int i = 0; i < 3; ++i)
    {
        system_.rhs()(ids[i]) +=
            fv * area / 3.0;
    }
}

void Poisson2D::applyDirichlet()
{
    applyDirichletSparse();
}

void Poisson2D::applyDirichletSparse()
{
    const int n =
        static_cast<int>(system_.size());

    if (n == 0)
    {
        return;
    }

    std::vector<char> is_boundary(
        static_cast<std::size_t>(n),
        0
    );

    std::vector<double> boundary_value(
        static_cast<std::size_t>(n),
        0.0
    );

    const std::vector<int> boundary_dofs =
        space_->boundaryDofs();

    for (int dof : boundary_dofs)
    {
        if (dof < 0 || dof >= n)
        {
            continue;
        }

        const auto p =
            space_->dofPoint(dof);

        is_boundary[static_cast<std::size_t>(dof)] = 1;

        boundary_value[static_cast<std::size_t>(dof)] =
            dirichlet_.value(
                p[0],
                p[1]
            );
    }

    const auto& K_old =
        system_.matrix();

    auto& F =
        system_.rhs();

    std::vector<Eigen::Triplet<double>> triplets;

    triplets.reserve(
        static_cast<std::size_t>(K_old.nonZeros()) +
        boundary_dofs.size()
    );

    for (int col = 0; col < K_old.outerSize(); ++col)
    {
        for (
            Eigen::SparseMatrix<double>::InnerIterator it(K_old, col);
            it;
            ++it
        )
        {
            const int row = it.row();
            const int c = it.col();
            const double value = it.value();

            const bool row_boundary =
                is_boundary[static_cast<std::size_t>(row)] != 0;

            const bool col_boundary =
                is_boundary[static_cast<std::size_t>(c)] != 0;

            if (!row_boundary && col_boundary)
            {
                F(row) -=
                    value *
                    boundary_value[static_cast<std::size_t>(c)];
            }

            if (!row_boundary && !col_boundary)
            {
                triplets.emplace_back(
                    row,
                    c,
                    value
                );
            }
        }
    }

    for (int dof : boundary_dofs)
    {
        if (dof < 0 || dof >= n)
        {
            continue;
        }

        triplets.emplace_back(
            dof,
            dof,
            1.0
        );

        F(dof) =
            boundary_value[static_cast<std::size_t>(dof)];
    }

    Eigen::SparseMatrix<double> K_new(
        n,
        n
    );

    K_new.setFromTriplets(
        triplets.begin(),
        triplets.end()
    );

    K_new.makeCompressed();

    system_.matrix() =
        std::move(K_new);
}

bool Poisson2D::solve()
{
    assemble();
    applyDirichlet();

    if (!solver_)
    {
        std::cerr << "[OpenCAX::Poisson2D] solver is null."
                  << std::endl;
        return false;
    }

    std::cout << "[OpenCAX::Poisson2D] dofs   = "
              << space_->numDofs()
              << std::endl;

    std::cout << "[OpenCAX::Poisson2D] cells  = "
              << space_->numCells()
              << std::endl;

    std::cout << "[OpenCAX::Poisson2D] solver = "
              << solver_->name()
              << std::endl;

    return solver_->solve(system_);
}

// void Poisson2D::showSolution(
//     const char* title
// ) const
// {
//     if (system_.solution().size() == 0)
//     {
//         std::cerr << "[OpenCAX::Poisson2D] solution is empty. "
//                   << "Call solve() first."
//                   << std::endl;
//         return;
//     }

//     ScalarFieldViewer::showSolution(
//         space_->mesh(),
//         system_.solution(),
//         title
//     );
// }

} // namespace OpenCAX
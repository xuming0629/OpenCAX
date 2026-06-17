#include <OpenCAX/FEM/Poisson2D.h>

#include <Eigen/SparseLU>

#include <set>
#include <vector>
#include <iostream>
#include <cmath>

namespace OpenCAX
{

Poisson2D::Poisson2D(const TriangleMesh& mesh)
    : mesh_(mesh)
{
    source_ = [](double, double) {
        return 1.0;
    };

    dirichlet_ = [](double, double) {
        return 0.0;
    };
}

void Poisson2D::setSource(Function2D f)
{
    source_ = std::move(f);
}

void Poisson2D::setDirichlet(Function2D g)
{
    dirichlet_ = std::move(g);
}

const Eigen::VectorXd& Poisson2D::solution() const
{
    return U_;
}

void Poisson2D::assemble()
{
    const std::size_t node_count = mesh_.num_nodes();
    const std::size_t tri_count = mesh_.num_triangles();

    K_.resize(node_count, node_count);
    F_ = Eigen::VectorXd::Zero(node_count);

    std::vector<Eigen::Triplet<double>> triplets;

    for (std::size_t cid = 0; cid < tri_count; ++cid)
    {
        auto tri = mesh_.triangle(cid);

        int id0 = tri[0];
        int id1 = tri[1];
        int id2 = tri[2];

        auto p0 = mesh_.point2d(id0);
        auto p1 = mesh_.point2d(id1);
        auto p2 = mesh_.point2d(id2);

        double x0 = p0[0];
        double y0 = p0[1];

        double x1 = p1[0];
        double y1 = p1[1];

        double x2 = p2[0];
        double y2 = p2[1];

        double detJ =
            (x1 - x0) * (y2 - y0) -
            (x2 - x0) * (y1 - y0);

        double area = 0.5 * std::abs(detJ);

        if (area < 1e-14)
        {
            continue;
        }

        double twoA = 2.0 * area;

        double b[3];
        double c[3];

        b[0] = (y1 - y2) / twoA;
        b[1] = (y2 - y0) / twoA;
        b[2] = (y0 - y1) / twoA;

        c[0] = (x2 - x1) / twoA;
        c[1] = (x0 - x2) / twoA;
        c[2] = (x1 - x0) / twoA;

        int ids[3] = {id0, id1, id2};

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                double kij = area * (b[i] * b[j] + c[i] * c[j]);
                triplets.emplace_back(ids[i], ids[j], kij);
            }
        }

        double xc = (x0 + x1 + x2) / 3.0;
        double yc = (y0 + y1 + y2) / 3.0;
        double fv = source_(xc, yc);

        for (int i = 0; i < 3; ++i)
        {
            F_(ids[i]) += fv * area / 3.0;
        }
    }

    K_.setFromTriplets(triplets.begin(), triplets.end());
}

void Poisson2D::applyDirichlet()
{
    auto boundary = mesh_.boundary_nodes();
    std::set<int> boundary_set(boundary.begin(), boundary.end());

    Eigen::MatrixXd dense = Eigen::MatrixXd(K_);

    for (int node_id : boundary_set)
    {
        auto p = mesh_.point2d(node_id);
        double value = dirichlet_(p[0], p[1]);

        for (int j = 0; j < dense.cols(); ++j)
        {
            dense(node_id, j) = 0.0;
        }

        for (int i = 0; i < dense.rows(); ++i)
        {
            dense(i, node_id) = 0.0;
        }

        dense(node_id, node_id) = 1.0;
        F_(node_id) = value;
    }

    K_ = dense.sparseView();
}

bool Poisson2D::solve()
{
    assemble();
    applyDirichlet();

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(K_);

    if (solver.info() != Eigen::Success)
    {
        std::cerr << "[OpenCAX::Poisson2D] matrix decomposition failed." << std::endl;
        return false;
    }

    U_ = solver.solve(F_);

    if (solver.info() != Eigen::Success)
    {
        std::cerr << "[OpenCAX::Poisson2D] linear solve failed." << std::endl;
        return false;
    }

    return true;
}

} // namespace OpenCAX
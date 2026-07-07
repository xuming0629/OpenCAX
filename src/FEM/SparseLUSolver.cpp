#include <OpenCAX/FEM/SparseLUSolver.h>

#include <Eigen/SparseLU>

#include <iostream>

namespace OpenCAX
{

bool SparseLUSolver::solve(LinearSystem& system)
{
    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(system.matrix());

    if (solver.info() != Eigen::Success)
    {
        std::cerr << "[OpenCAX::SparseLUSolver] matrix decomposition failed."
                  << std::endl;
        return false;
    }

    system.solution() = solver.solve(system.rhs());

    if (solver.info() != Eigen::Success)
    {
        std::cerr << "[OpenCAX::SparseLUSolver] linear solve failed."
                  << std::endl;
        return false;
    }

    return true;
}

} // namespace OpenCAX

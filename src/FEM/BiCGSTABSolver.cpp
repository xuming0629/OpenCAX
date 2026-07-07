#include <OpenCAX/FEM/BiCGSTABSolver.h>

#include <Eigen/IterativeLinearSolvers>

#include <iostream>

namespace OpenCAX
{

bool BiCGSTABSolver::solve(LinearSystem& system)
{
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver;

    solver.setTolerance(tolerance_);
    solver.setMaxIterations(max_iterations_);
    solver.compute(system.matrix());

    if (solver.info() != Eigen::Success)
    {
        std::cerr << "[OpenCAX::BiCGSTABSolver] matrix decomposition failed."
                  << std::endl;
        return false;
    }

    system.solution() = solver.solve(system.rhs());
    iterations_ = static_cast<int>(solver.iterations());
    error_ = solver.error();

    if (solver.info() != Eigen::Success)
    {
        std::cerr << "[OpenCAX::BiCGSTABSolver] linear solve failed."
                  << std::endl;
        return false;
    }

    std::cout << "[OpenCAX::BiCGSTABSolver] iterations = "
              << iterations_
              << ", estimated error = "
              << error_
              << std::endl;

    return true;
}

} // namespace OpenCAX

#include <OpenCAX/FEM/CGSolver.h>

#include <Eigen/IterativeLinearSolvers>

#include <iostream>

namespace OpenCAX
{

bool CGSolver::solve(LinearSystem& system)
{
    Eigen::ConjugateGradient<
        Eigen::SparseMatrix<double>,
        Eigen::Lower | Eigen::Upper
    > solver;

    solver.setTolerance(tolerance_);
    solver.setMaxIterations(max_iterations_);
    solver.compute(system.matrix());

    if (solver.info() != Eigen::Success)
    {
        std::cerr << "[OpenCAX::CGSolver] matrix decomposition failed."
                  << std::endl;
        return false;
    }

    system.solution() = solver.solve(system.rhs());
    iterations_ = static_cast<int>(solver.iterations());
    error_ = solver.error();

    if (solver.info() != Eigen::Success)
    {
        std::cerr << "[OpenCAX::CGSolver] linear solve failed."
                  << std::endl;
        return false;
    }

    std::cout << "[OpenCAX::CGSolver] iterations = "
              << iterations_
              << ", estimated error = "
              << error_
              << std::endl;

    return true;
}

} // namespace OpenCAX

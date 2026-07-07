#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/LinearSolver.h>

namespace OpenCAX
{

class OpenCAX_API CGSolver : public LinearSolver
{
public:
    CGSolver() = default;

    void setTolerance(double tol) { tolerance_ = tol; }
    void setMaxIterations(int max_iter) { max_iterations_ = max_iter; }

    double tolerance() const { return tolerance_; }
    int maxIterations() const { return max_iterations_; }
    int iterations() const { return iterations_; }
    double error() const { return error_; }

    bool solve(LinearSystem& system) override;

    SolverType type() const override
    {
        return SolverType::CG;
    }

    std::string name() const override
    {
        return "Eigen::ConjugateGradient";
    }

private:
    double tolerance_ = 1e-12;
    int max_iterations_ = 10000;
    int iterations_ = 0;
    double error_ = 0.0;
};

} // namespace OpenCAX

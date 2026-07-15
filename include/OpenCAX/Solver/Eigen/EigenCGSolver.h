#pragma once
#include <OpenCAX/Solver/LinearSolver.h>
namespace OpenCAX {
class EigenCGSolver final : public LinearSolver {
public:
    const char* name() const override { return "Eigen::ConjugateGradient"; }
    SolverResult solve(LinearSystem& system, const SolverOptions& options) override;
};
} // namespace OpenCAX

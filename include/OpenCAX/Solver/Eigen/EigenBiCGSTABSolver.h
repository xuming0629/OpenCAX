#pragma once
#include <OpenCAX/Solver/LinearSolver.h>
namespace OpenCAX {
class EigenBiCGSTABSolver final : public LinearSolver {
public:
    const char* name() const override { return "Eigen::BiCGSTAB"; }
    SolverResult solve(LinearSystem& system, const SolverOptions& options) override;
};
} // namespace OpenCAX

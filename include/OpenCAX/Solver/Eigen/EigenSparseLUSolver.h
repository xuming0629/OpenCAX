#pragma once
#include <OpenCAX/Solver/LinearSolver.h>
namespace OpenCAX {
class EigenSparseLUSolver final : public LinearSolver {
public:
    const char* name() const override { return "Eigen::SparseLU"; }
    SolverResult solve(LinearSystem& system, const SolverOptions& options) override;
};
} // namespace OpenCAX

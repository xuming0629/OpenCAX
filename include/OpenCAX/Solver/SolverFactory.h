#pragma once
#include <OpenCAX/Solver/LinearSolver.h>
#include <memory>
namespace OpenCAX {
class SolverFactory {
public:
    static std::unique_ptr<LinearSolver> create(const SolverOptions& options);
};
} // namespace OpenCAX

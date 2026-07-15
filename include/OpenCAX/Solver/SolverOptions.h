#pragma once
#include <OpenCAX/Solver/SolverTypes.h>
namespace OpenCAX {
struct SolverOptions {
    SolverBackend backend = SolverBackend::Eigen;
    SolverMethod method = SolverMethod::SparseLU;
    PreconditionerType preconditioner = PreconditionerType::None;
    int max_iterations = 1000;
    double tolerance = 1.0e-10;
    bool verbose = false;
};
} // namespace OpenCAX

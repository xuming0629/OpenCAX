#pragma once
#include <OpenCAX/Solver/SolverTypes.h>

namespace OpenCAX {

struct SolverOptions
{
    SolverBackend backend = SolverBackend::Eigen;
    SolverMethod method = SolverMethod::SparseLU;
    SolverExecution execution = SolverExecution::Serial;

    PreconditionerType preconditioner =
        PreconditionerType::None;

    int max_iterations = 1000;
    double tolerance = 1e-10;

    int mpi_rank = 0;
    int mpi_size = 1;

    int gpu_device = 0;

    bool distributed = false;
    bool verbose = false;
};

}

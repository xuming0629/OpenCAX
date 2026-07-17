#pragma once
namespace OpenCAX {

enum class SolverBackend
{
    Eigen,
    MUMPS,
    PETSc,
    CUDA
};

enum class SolverMethod
{
    SparseLU,
    CG,
    BiCGSTAB
};

enum class SolverExecution
{
    Serial,
    MPI
};

enum class PreconditionerType
{
    None,
    Diagonal,
    ILU,
    AMG
};

enum class SolverStatus
{
    Success,
    Failed,
    MaxIterations,
    InvalidSystem,
    Unsupported
};

}

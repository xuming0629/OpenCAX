#pragma once
namespace OpenCAX {
enum class SolverBackend { Eigen, MUMPS, PETSc, CUDA };
enum class SolverMethod { SparseLU, CG, BiCGSTAB };
enum class PreconditionerType { None, Diagonal, ILU };
enum class SolverStatus { Success, Failed, MaxIterations, InvalidSystem, Unsupported };
} // namespace OpenCAX

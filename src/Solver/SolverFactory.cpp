#include <OpenCAX/Solver/SolverFactory.h>

#include <OpenCAX/Solver/Eigen/EigenSparseLUSolver.h>
#include <OpenCAX/Solver/Eigen/EigenCGSolver.h>
#include <OpenCAX/Solver/Eigen/EigenBiCGSTABSolver.h>


#ifdef OPEN_CAX_HAS_MUMPS
#include <OpenCAX/Solver/Mumps/MumpsSolver.h>
#endif


#ifdef OPEN_CAX_HAS_PETSC
#include <OpenCAX/Solver/PETSc/PetscSolver.h>
#endif


#ifdef OPEN_CAX_HAS_CUDA
#include <OpenCAX/Solver/CUDA/CudaSolver.h>
#endif


#include <stdexcept>


namespace OpenCAX
{


std::unique_ptr<LinearSolver>
SolverFactory::create(
    const SolverOptions& o
)
{

    switch(o.backend)
    {


    // ====================================================
    // Eigen
    // ====================================================

    case SolverBackend::Eigen:
    {

        switch(o.method)
        {

        case SolverMethod::SparseLU:

            return std::make_unique<
                EigenSparseLUSolver
            >();


        case SolverMethod::CG:

            return std::make_unique<
                EigenCGSolver
            >();


        case SolverMethod::BiCGSTAB:

            return std::make_unique<
                EigenBiCGSTABSolver
            >();

        }

        break;

    }



    // ====================================================
    // MUMPS
    // ====================================================

    case SolverBackend::MUMPS:
    {

#ifdef OPEN_CAX_HAS_MUMPS

        return std::make_unique<
            MumpsSolver
        >();

#else

        throw std::runtime_error(
            "MUMPS backend is not enabled"
        );

#endif

    }



    // ====================================================
    // PETSc
    // ====================================================

    case SolverBackend::PETSc:
    {

#ifdef OPEN_CAX_HAS_PETSC

        return std::make_unique<
            PetscSolver
        >();

#else

        throw std::runtime_error(
            "PETSc backend is not enabled"
        );

#endif

    }



    // ====================================================
    // CUDA
    // ====================================================

    case SolverBackend::CUDA:
    {

#ifdef OPEN_CAX_HAS_CUDA

        return std::make_unique<
            CudaSolver
        >();

#else

        throw std::runtime_error(
            "CUDA backend is not enabled"
        );

#endif

    }


    }


    throw std::runtime_error(
        "Unsupported solver configuration"
    );

}


}
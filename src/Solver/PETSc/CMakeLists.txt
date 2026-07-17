# ============================================================
# PETSc Solver Backend
# ============================================================


add_library(
    OpenCAXSolverPETSc

    PetscSolver.cpp

)



add_library(
    OpenCAX::SolverPETSc

    ALIAS

    OpenCAXSolverPETSc

)



target_include_directories(
    OpenCAXSolverPETSc

    PUBLIC

    ${PROJECT_SOURCE_DIR}/include

)



target_link_libraries(
    OpenCAXSolverPETSc

    PUBLIC

    MPI::MPI_CXX

    PETSc::PETSc

)



target_compile_definitions(
    OpenCAXSolverPETSc

    PUBLIC

    OPEN_CAX_HAS_PETSC
    OPEN_CAX_HAS_MPI

)
# ============================================================
# OpenCAX Solver Module
# ============================================================


# ============================================================
# Solver Core
# ============================================================


add_library(
    OpenCAXSolver

    SolverFactory.cpp

)



add_library(
    OpenCAX::Solver

    ALIAS

    OpenCAXSolver
)



target_include_directories(
    OpenCAXSolver

    PUBLIC

    ${PROJECT_SOURCE_DIR}/include
)



# ============================================================
# Eigen Solver Backend
# ============================================================


if(OPEN_CAX_SOLVER_USE_EIGEN)

    add_subdirectory(Eigen)


    target_link_libraries(
        OpenCAXSolver

        PUBLIC

        OpenCAX::SolverEigen
    )


endif()



# ============================================================
# MUMPS Solver Backend
# ============================================================


if(OPEN_CAX_SOLVER_USE_MUMPS)

    add_subdirectory(Mumps)


    target_link_libraries(
        OpenCAXSolver

        PUBLIC

        OpenCAX::SolverMumps
    )


endif()



# ============================================================
# PETSc Solver Backend
# ============================================================


if(OPEN_CAX_SOLVER_USE_PETSC)

    add_subdirectory(PETSc)


    target_link_libraries(
        OpenCAXSolver

        PUBLIC

        OpenCAX::SolverPETSc
    )


endif()



# ============================================================
# CUDA Solver Backend
# ============================================================


if(OPEN_CAX_SOLVER_USE_CUDA)

    add_subdirectory(CUDA)


    target_link_libraries(
        OpenCAXSolver

        PUBLIC

        OpenCAX::SolverCUDA
    )


endif()
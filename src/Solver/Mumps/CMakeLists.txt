# ============================================================
# MUMPS Solver Backend
# ============================================================


add_library(
    OpenCAXSolverMumps

    MumpsSolver.cpp

)



add_library(
    OpenCAX::SolverMumps
    ALIAS
    OpenCAXSolverMumps
)



target_include_directories(
    OpenCAXSolverMumps

    PUBLIC

    ${PROJECT_SOURCE_DIR}/include

)



target_link_libraries(
    OpenCAXSolverMumps

    PUBLIC

    MPI::MPI_CXX

    MUMPS::DMUMPS

)



target_compile_definitions(
    OpenCAXSolverMumps

    PUBLIC

    OPEN_CAX_HAS_MUMPS
    OPEN_CAX_HAS_MPI

)
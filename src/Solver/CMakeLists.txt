# ============================================================
# OpenCAX PETSc Solver Backend
# ============================================================

if(NOT OPEN_CAX_SOLVER_USE_PETSC)
    return()
endif()

find_package(MPI REQUIRED COMPONENTS C CXX)
find_package(PETSc REQUIRED)

add_library(OpenCAXSolverPETSc
    PetscLinearSolver.cpp
)

add_library(OpenCAX::SolverPETSc
    ALIAS OpenCAXSolverPETSc
)

target_compile_features(OpenCAXSolverPETSc
    PUBLIC
        cxx_std_17
)

target_include_directories(OpenCAXSolverPETSc
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(OpenCAXSolverPETSc
    PUBLIC
        OpenCAX::SolverCore
        MPI::MPI_CXX
        PETSc::PETSc
)

target_compile_definitions(OpenCAXSolverPETSc
    PUBLIC
        OPEN_CAX_HAS_PETSC=1
        OPEN_CAX_HAS_MPI=1
)

set_target_properties(OpenCAXSolverPETSc PROPERTIES
    EXPORT_NAME SolverPETSc
)

install(
    TARGETS OpenCAXSolverPETSc
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
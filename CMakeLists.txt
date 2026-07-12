cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()

project(OpenCAX
    VERSION 0.1.0
    DESCRIPTION "OpenCAX scientific computing and engineering simulation framework"
    LANGUAGES CXX
)

# ============================================================
# Global C++ Configuration
# ============================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

# ============================================================
# Library Type
# ============================================================

option(BUILD_SHARED_LIBS "Build OpenCAX libraries as shared libraries" OFF)

# ============================================================
# Main Modules
# ============================================================

option(OPEN_CAX_BUILD_LOGGING "Build OpenCAX logging module" ON)
option(OPEN_CAX_BUILD_MESH "Build OpenCAX mesh module" ON)
option(OPEN_CAX_BUILD_MESHING "Build OpenCAX meshing module" ON)
option(OPEN_CAX_BUILD_SOLVER "Build OpenCAX solver module" ON)
option(OPEN_CAX_BUILD_FEM "Build OpenCAX finite element module" ON)
option(OPEN_CAX_BUILD_POST "Build OpenCAX post-processing module" ON)

# ============================================================
# Solver Backends
# ============================================================

option(OPEN_CAX_SOLVER_USE_EIGEN "Enable Eigen solver backend" ON)
option(OPEN_CAX_SOLVER_USE_MUMPS "Enable MUMPS solver backend" OFF)
option(OPEN_CAX_SOLVER_USE_PETSC "Enable PETSc solver backend" OFF)
option(OPEN_CAX_SOLVER_USE_CUDA "Enable CUDA solver backend" OFF)
option(OPEN_CAX_SOLVER_USE_MPI "Enable MPI support for solver backends" OFF)

# ============================================================
# Meshing Backends
# ============================================================

option(OPEN_CAX_MESH_USE_GMSH "Enable Gmsh meshing backend" ON)
option(OPEN_CAX_MESH_USE_TETGEN "Enable TetGen meshing backend" ON)
option(OPEN_CAX_MESH_USE_NETGEN "Enable Netgen meshing backend" OFF)

# ============================================================
# Auxiliary Options
# ============================================================

option(OPEN_CAX_BUILD_EXAMPLES "Build OpenCAX examples" ON)
option(OPEN_CAX_BUILD_TESTS "Build OpenCAX tests" ON)
option(OPEN_CAX_BUILD_PYTHON "Build OpenCAX Python bindings" OFF)
option(OPEN_CAX_BUILD_DOCS "Build OpenCAX API documentation" OFF)

# ============================================================
# Option Dependency Validation
# ============================================================

if(OPEN_CAX_BUILD_MESHING AND NOT OPEN_CAX_BUILD_MESH)
    message(FATAL_ERROR "OPEN_CAX_BUILD_MESHING=ON requires OPEN_CAX_BUILD_MESH=ON")
endif()

if(OPEN_CAX_BUILD_FEM AND NOT OPEN_CAX_BUILD_MESH)
    message(FATAL_ERROR "OPEN_CAX_BUILD_FEM=ON requires OPEN_CAX_BUILD_MESH=ON")
endif()

if(OPEN_CAX_BUILD_FEM AND NOT OPEN_CAX_BUILD_SOLVER)
    message(FATAL_ERROR "OPEN_CAX_BUILD_FEM=ON requires OPEN_CAX_BUILD_SOLVER=ON")
endif()

if(OPEN_CAX_BUILD_POST AND NOT OPEN_CAX_BUILD_MESH)
    message(FATAL_ERROR "OPEN_CAX_BUILD_POST=ON requires OPEN_CAX_BUILD_MESH=ON")
endif()

if(OPEN_CAX_BUILD_SOLVER AND NOT OPEN_CAX_SOLVER_USE_EIGEN AND NOT OPEN_CAX_SOLVER_USE_MUMPS AND NOT OPEN_CAX_SOLVER_USE_PETSC AND NOT OPEN_CAX_SOLVER_USE_CUDA)
    message(FATAL_ERROR "OPEN_CAX_BUILD_SOLVER=ON requires at least one solver backend")
endif()

if(OPEN_CAX_SOLVER_USE_PETSC AND NOT OPEN_CAX_SOLVER_USE_MPI)
    message(STATUS "PETSc backend enabled: automatically enabling MPI support")
    set(OPEN_CAX_SOLVER_USE_MPI ON CACHE BOOL "Enable MPI support for solver backends" FORCE)
endif()

if(NOT OPEN_CAX_BUILD_SOLVER)
    set(OPEN_CAX_SOLVER_USE_EIGEN OFF CACHE BOOL "Enable Eigen solver backend" FORCE)
    set(OPEN_CAX_SOLVER_USE_MUMPS OFF CACHE BOOL "Enable MUMPS solver backend" FORCE)
    set(OPEN_CAX_SOLVER_USE_PETSC OFF CACHE BOOL "Enable PETSc solver backend" FORCE)
    set(OPEN_CAX_SOLVER_USE_CUDA OFF CACHE BOOL "Enable CUDA solver backend" FORCE)
    set(OPEN_CAX_SOLVER_USE_MPI OFF CACHE BOOL "Enable MPI support for solver backends" FORCE)
endif()

if(NOT OPEN_CAX_BUILD_MESHING)
    set(OPEN_CAX_MESH_USE_GMSH OFF CACHE BOOL "Enable Gmsh meshing backend" FORCE)
    set(OPEN_CAX_MESH_USE_TETGEN OFF CACHE BOOL "Enable TetGen meshing backend" FORCE)
    set(OPEN_CAX_MESH_USE_NETGEN OFF CACHE BOOL "Enable Netgen meshing backend" FORCE)
endif()

# ============================================================
# CMake Modules
# ============================================================

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ============================================================
# Dependencies
# ============================================================

include(OpenCAXDependencies)

# ============================================================
# Source Modules
# ============================================================

add_subdirectory(src)

# ============================================================
# Examples
# ============================================================

if(OPEN_CAX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ============================================================
# Tests
# ============================================================

if(OPEN_CAX_BUILD_TESTS)
    include(CTest)

    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

# ============================================================
# Python Bindings
# ============================================================

if(OPEN_CAX_BUILD_PYTHON)
    add_subdirectory(python)
endif()

# ============================================================
# Documentation
# ============================================================

if(OPEN_CAX_BUILD_DOCS)
    add_subdirectory(docs)
endif()

# ============================================================
# Configuration Summary
# ============================================================

message(STATUS "")
message(STATUS "============================================================")
message(STATUS "OpenCAX ${PROJECT_VERSION} configuration")
message(STATUS "============================================================")
message(STATUS "Build type              : ${CMAKE_BUILD_TYPE}")
message(STATUS "Shared libraries        : ${BUILD_SHARED_LIBS}")
message(STATUS "")
message(STATUS "Modules:")
message(STATUS "  Logging               : ${OPEN_CAX_BUILD_LOGGING}")
message(STATUS "  Mesh                  : ${OPEN_CAX_BUILD_MESH}")
message(STATUS "  Meshing               : ${OPEN_CAX_BUILD_MESHING}")
message(STATUS "  Solver                : ${OPEN_CAX_BUILD_SOLVER}")
message(STATUS "  FEM                   : ${OPEN_CAX_BUILD_FEM}")
message(STATUS "  Post                  : ${OPEN_CAX_BUILD_POST}")
message(STATUS "")
message(STATUS "Solver backends:")
message(STATUS "  Eigen                 : ${OPEN_CAX_SOLVER_USE_EIGEN}")
message(STATUS "  MUMPS                 : ${OPEN_CAX_SOLVER_USE_MUMPS}")
message(STATUS "  PETSc                 : ${OPEN_CAX_SOLVER_USE_PETSC}")
message(STATUS "  CUDA                  : ${OPEN_CAX_SOLVER_USE_CUDA}")
message(STATUS "  MPI                   : ${OPEN_CAX_SOLVER_USE_MPI}")
message(STATUS "")
message(STATUS "Meshing backends:")
message(STATUS "  Gmsh                  : ${OPEN_CAX_MESH_USE_GMSH}")
message(STATUS "  TetGen                : ${OPEN_CAX_MESH_USE_TETGEN}")
message(STATUS "  Netgen                : ${OPEN_CAX_MESH_USE_NETGEN}")
message(STATUS "")
message(STATUS "Auxiliary:")
message(STATUS "  Examples              : ${OPEN_CAX_BUILD_EXAMPLES}")
message(STATUS "  Tests                 : ${OPEN_CAX_BUILD_TESTS}")
message(STATUS "  Python                : ${OPEN_CAX_BUILD_PYTHON}")
message(STATUS "  Documentation         : ${OPEN_CAX_BUILD_DOCS}")
message(STATUS "============================================================")
message(STATUS "")


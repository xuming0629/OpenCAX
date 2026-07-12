cmake_minimum_required(VERSION 3.16)

cmake_policy(SET CMP0144 NEW)

project(OpenCAX
    VERSION 0.1.0
    LANGUAGES CXX
)

# =========================================
# C++
# =========================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =========================================
# Build Options
# =========================================
option(OPEN_CAX_BUILD_EXAMPLES "Build examples" ON)
option(OPEN_CAX_BUILD_TESTS    "Build tests" ON)
option(OPEN_CAX_BUILD_LOGGING  "Build logging module" ON)


option(OPEN_CAX_BUILD_MESH      "Build old mesh module" ON)
option(OPEN_CAX_BUILD_MESHING   "Build meshing module" ON)


option(OPEN_CAX_BUILD_POST     "Build post-processing module" ON)
option(OPEN_CAX_BUILD_FEM      "Build FEM module" ON)


option(OPEN_CAX_BUILD_PYTHON   "Build python bindings" ON)

option(OPEN_CAX_BUILD_DOCS "Build API documentation" ON)

option(OPEN_CAX_USE_EIGEN3 "Enable Eigen3 support" ON)
# =========================================
# Backend Options
# =========================================
option(OPEN_CAX_ENABLE_GMSH   "Enable Gmsh mesh backend" ON)
option(OPEN_CAX_ENABLE_TETGEN "Enable TetGen mesh backend" ON)
option(OPEN_CAX_ENABLE_NETGEN "Enable Netgen mesh backend" OFF)

# =========================================
# CMake Modules
# =========================================
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

# =========================================
# Dependencies
# =========================================
include(OpenCAXDependencies)

# =========================================
# Source
# =========================================
add_subdirectory(src)

# =========================================
# Examples
# =========================================
if(OPEN_CAX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# =========================================
# Tests
# =========================================
if(OPEN_CAX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()


if(OPEN_CAX_BUILD_DOCS)
    add_subdirectory(docs)
endif()
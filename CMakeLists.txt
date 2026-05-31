cmake_minimum_required(VERSION 3.16)
project(OpenCAX VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(OPEN_CAX_BUILD_EXAMPLES "Build examples" ON)
option(OPEN_CAX_BUILD_TESTS "Build tests" ON)

# ------------------------------
# OpenCASCADE
# ------------------------------
set(OpenCASCADE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/occt-7.9.2/lib/cmake/opencascade")
find_package(OpenCASCADE REQUIRED)

message(STATUS "OpenCASCADE include: ${OpenCASCADE_INCLUDE_DIR}")
message(STATUS "OpenCASCADE libs: ${OpenCASCADE_LIBRARIES}")
if(NOT OpenCASCADE_FOUND)
    message(FATAL_ERROR "Could not find OpenCASCADE")
else()
    message(STATUS "Using OpenCASCADE from ${OpenCASCADE_INSTALL_PREFIX}")
endif()

include_directories(
    3rdparty/occt-7.9.2/include
    3rdparty/occt-7.9.2/include/opencascade
)
link_directories(3rdparty/occt-7.9.2/lib)
# ------------------------------
# VTK
# ------------------------------
set(VTK_DIR "${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/lib/cmake/vtk-9.4")
find_package(VTK REQUIRED)

message(STATUS "VTK include: ${VTK_INCLUDE_DIRS}")
message(STATUS "VTK libs: ${VTK_LIBRARIES}")
if(NOT VTK_FOUND)
    message(FATAL_ERROR "Could not find VTK")
endif()
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/include/vtk-9.3)
link_directories(${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/lib)


# ------------------------------
# Gmsh
# ------------------------------
set(GMSH_ROOT "${CMAKE_SOURCE_DIR}/3rdparty/gmsh-4.15")

include_directories(${GMSH_ROOT}/include)
link_directories(${GMSH_ROOT}/lib)

find_library(GMSH_LIBRARY
    NAMES gmsh
    PATHS ${GMSH_ROOT}/lib
    REQUIRED
)

message(STATUS "Gmsh include: ${GMSH_ROOT}/include")
message(STATUS "Gmsh lib: ${GMSH_LIBRARY}")

add_subdirectory(src)


if(OPEN_CAX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(OPEN_CAX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

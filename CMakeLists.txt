cmake_minimum_required(VERSION 3.16)

project(OpenCAX
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(OPEN_CAX_BUILD_EXAMPLES "Build examples" ON)
option(OPEN_CAX_BUILD_TESTS "Build tests" ON)
option(OPEN_CAX_BUILD_MESHING "Build meshing module" ON)
option(OPEN_CAX_BUILD_VISUALIZATION "Build visualization module" ON)
option(OPEN_CAX_BUILD_FEM "Build FEM module" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(OpenCAXDependencies)

add_subdirectory(src)

if(OPEN_CAX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(OPEN_CAX_BUILD_TESTS)
    # enable_testing()
    # add_subdirectory(tests)
endif()
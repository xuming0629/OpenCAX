# ==============================
# OpenCASCADE
# ==============================
if(NOT OpenCASCADE_DIR)
    set(OpenCASCADE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/occt-7.9.2/lib/cmake/opencascade")
endif()

find_package(OpenCASCADE REQUIRED)

if(NOT OpenCASCADE_FOUND)
    message(FATAL_ERROR "Could not find OpenCASCADE")
endif()

message(STATUS "OpenCASCADE include: ${OpenCASCADE_INCLUDE_DIR}")
message(STATUS "OpenCASCADE libs: ${OpenCASCADE_LIBRARIES}")
message(STATUS "Using OpenCASCADE from ${OpenCASCADE_INSTALL_PREFIX}")


# ==============================
# VTK
# ==============================
if(OPEN_CAX_BUILD_VISUALIZATION)
    if(NOT VTK_DIR)
        set(VTK_DIR "${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/lib/cmake/vtk-9.4")
    endif()

    find_package(VTK REQUIRED)

    if(NOT VTK_FOUND)
        message(FATAL_ERROR "Could not find VTK")
    endif()

    message(STATUS "VTK include: ${VTK_INCLUDE_DIRS}")
    message(STATUS "VTK libs: ${VTK_LIBRARIES}")
endif()


# ==============================
# Gmsh
# ==============================
if(OPEN_CAX_BUILD_MESHING)
    if(NOT GMSH_ROOT)
        set(GMSH_ROOT "${CMAKE_SOURCE_DIR}/3rdparty/gmsh-4.15")
    endif()

    find_path(GMSH_INCLUDE_DIR
        NAMES gmsh.h
        PATHS ${GMSH_ROOT}/include
        REQUIRED
    )

    find_library(GMSH_LIBRARY
        NAMES gmsh
        PATHS ${GMSH_ROOT}/lib
        REQUIRED
    )

    message(STATUS "Gmsh include: ${GMSH_INCLUDE_DIR}")
    message(STATUS "Gmsh lib: ${GMSH_LIBRARY}")
endif()
# ==============================
# GoogleTest
# ==============================
if(OPEN_CAX_BUILD_TESTS)

    if(NOT GTEST_ROOT)
        set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/3rdparty/googletest")
    endif()

    find_path(GTEST_INCLUDE_DIR
        NAMES gtest/gtest.h
        PATHS ${GTEST_ROOT}/include
        REQUIRED
    )

    find_library(GTEST_LIBRARY
        NAMES gtest
        PATHS ${GTEST_ROOT}/lib
        REQUIRED
    )

    find_library(GTEST_MAIN_LIBRARY
        NAMES gtest_main
        PATHS ${GTEST_ROOT}/lib
        REQUIRED
    )

    message(STATUS "GoogleTest include: ${GTEST_INCLUDE_DIR}")
    message(STATUS "GoogleTest lib: ${GTEST_LIBRARY}")
    message(STATUS "GoogleTest main lib: ${GTEST_MAIN_LIBRARY}")

endif()


# ==============================
# spdlog
# ==============================
if(OPEN_CAX_BUILD_LOGGING)

    if(NOT SPDLOG_ROOT)
        set(SPDLOG_ROOT "${CMAKE_SOURCE_DIR}/3rdparty/spdlog")
    endif()

    find_path(SPDLOG_INCLUDE_DIR
        NAMES spdlog/spdlog.h
        PATHS ${SPDLOG_ROOT}/include
        REQUIRED
    )

    find_library(SPDLOG_LIBRARY
        NAMES spdlog
        PATHS ${SPDLOG_ROOT}/lib
    )

    message(STATUS "spdlog include: ${SPDLOG_INCLUDE_DIR}")
    message(STATUS "spdlog lib: ${SPDLOG_LIBRARY}")

endif()
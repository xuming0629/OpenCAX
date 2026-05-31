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
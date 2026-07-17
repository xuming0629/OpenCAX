# # ============================================================
# # OpenCAX Third-party Dependencies
# # ============================================================


# # ============================================================
# # OpenCASCADE
# # ============================================================

# if(NOT OpenCASCADE_DIR)

#     set(
#         OpenCASCADE_DIR
#         "${CMAKE_SOURCE_DIR}/3rdparty/occt-7.9.2/lib/cmake/opencascade"
#         CACHE PATH
#         "OpenCASCADE CMake package directory"
#     )

# endif()


# find_package(OpenCASCADE REQUIRED)


# message(
#     STATUS
#     "OpenCASCADE include: ${OpenCASCADE_INCLUDE_DIR}"
# )

# message(
#     STATUS
#     "OpenCASCADE libs: ${OpenCASCADE_LIBRARIES}"
# )

# message(
#     STATUS
#     "OpenCASCADE prefix: ${OpenCASCADE_INSTALL_PREFIX}"
# )



# # ============================================================
# # VTK
# # Used by Post
# # ============================================================

# if(OPEN_CAX_BUILD_POST)


#     if(NOT VTK_DIR)

#         set(
#             VTK_DIR
#             "${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/lib/cmake/vtk-9.4"
#             CACHE PATH
#             "VTK CMake package directory"
#         )

#     endif()


#     find_package(VTK REQUIRED)


#     message(
#         STATUS
#         "VTK include: ${VTK_INCLUDE_DIRS}"
#     )

#     message(
#         STATUS
#         "VTK libs: ${VTK_LIBRARIES}"
#     )


# endif()



# # ============================================================
# # Gmsh
# # ============================================================

# if(OPEN_CAX_BUILD_MESH AND OPEN_CAX_MESH_USE_GMSH)


#     if(NOT GMSH_ROOT)

#         set(
#             GMSH_ROOT
#             "${CMAKE_SOURCE_DIR}/3rdparty/gmsh-4.15"
#             CACHE PATH
#             "Gmsh root directory"
#         )

#     endif()



#     find_path(
#         GMSH_INCLUDE_DIR
#         NAMES gmsh.h
#         PATHS ${GMSH_ROOT}/include
#         REQUIRED
#     )


#     find_library(
#         GMSH_LIBRARY
#         NAMES gmsh
#         PATHS ${GMSH_ROOT}/lib
#         REQUIRED
#     )


#     message(
#         STATUS
#         "Gmsh include: ${GMSH_INCLUDE_DIR}"
#     )


#     message(
#         STATUS
#         "Gmsh lib: ${GMSH_LIBRARY}"
#     )



#     if(NOT TARGET Gmsh::Gmsh)

#         add_library(
#             Gmsh::Gmsh
#             UNKNOWN
#             IMPORTED
#         )


#         set_target_properties(
#             Gmsh::Gmsh

#             PROPERTIES

#             IMPORTED_LOCATION
#             "${GMSH_LIBRARY}"

#             INTERFACE_INCLUDE_DIRECTORIES
#             "${GMSH_INCLUDE_DIR}"
#         )

#     endif()


# endif()



# # ============================================================
# # TetGen
# # ============================================================

# if(OPEN_CAX_BUILD_MESH AND OPEN_CAX_MESH_USE_TETGEN)


#     if(NOT TETGEN_ROOT)

#         set(
#             TETGEN_ROOT
#             "${CMAKE_SOURCE_DIR}/3rdparty/tetgen"
#             CACHE PATH
#             "TetGen root directory"
#         )

#     endif()



#     find_path(
#         TETGEN_INCLUDE_DIR
#         NAMES tetgen.h
#         PATHS
#             ${TETGEN_ROOT}
#             ${TETGEN_ROOT}/include
#         REQUIRED
#     )


#     find_file(
#         TETGEN_SOURCE_FILE
#         NAMES tetgen.cxx
#         PATHS
#             ${TETGEN_ROOT}
#             ${TETGEN_ROOT}/src
#         REQUIRED
#     )


#     find_file(
#         TETGEN_PREDICATES_FILE
#         NAMES predicates.cxx
#         PATHS
#             ${TETGEN_ROOT}
#             ${TETGEN_ROOT}/src
#         REQUIRED
#     )



#     if(NOT TARGET TetGen::TetGen)


#         add_library(
#             OpenCAXTetGen

#             ${TETGEN_SOURCE_FILE}
#             ${TETGEN_PREDICATES_FILE}

#         )


#         add_library(
#             TetGen::TetGen
#             ALIAS
#             OpenCAXTetGen
#         )


#         target_include_directories(
#             OpenCAXTetGen

#             PUBLIC

#             ${TETGEN_INCLUDE_DIR}
#         )


#         target_compile_definitions(
#             OpenCAXTetGen

#             PUBLIC

#             TETLIBRARY

#         )


#     endif()


# endif()



# # ============================================================
# # GoogleTest
# # ============================================================

# if(OPEN_CAX_BUILD_TESTS)


#     if(NOT GTEST_ROOT)

#         set(
#             GTEST_ROOT
#             "${CMAKE_SOURCE_DIR}/3rdparty/googletest"
#             CACHE PATH
#             "GoogleTest root"
#         )

#     endif()



#     find_package(GTest REQUIRED)


# endif()



# # ============================================================
# # spdlog
# # ============================================================

# if(OPEN_CAX_BUILD_LOGGING)


#     if(NOT SPDLOG_ROOT)

#         set(
#             SPDLOG_ROOT
#             "${CMAKE_SOURCE_DIR}/3rdparty/spdlog"
#             CACHE PATH
#             "spdlog root"
#         )

#     endif()



#     find_path(
#         SPDLOG_INCLUDE_DIR
#         NAMES spdlog/spdlog.h
#         PATHS
#         ${SPDLOG_ROOT}/include
#         REQUIRED
#     )



#     if(NOT TARGET spdlog::spdlog)

#         add_library(
#             spdlog::spdlog
#             INTERFACE IMPORTED
#         )


#         set_target_properties(
#             spdlog::spdlog

#             PROPERTIES

#             INTERFACE_INCLUDE_DIRECTORIES
#             "${SPDLOG_INCLUDE_DIR}"

#         )

#     endif()


# endif()



# # ============================================================
# # Eigen3
# # Used by FEM / Eigen Solver
# # ============================================================


# option(
#     OPEN_CAX_USE_EIGEN3
#     "Enable Eigen3 support"
#     ON
# )



# if(
#     OPEN_CAX_BUILD_FEM
#     OR OPEN_CAX_SOLVER_USE_EIGEN
# )


#     if(NOT OPEN_CAX_USE_EIGEN3)

#         message(
#             FATAL_ERROR
#             "FEM or Eigen solver requires Eigen3"
#         )

#     endif()


# endif()



# if(OPEN_CAX_USE_EIGEN3)


#     if(NOT EIGEN3_ROOT)

#         set(
#             EIGEN3_ROOT
#             "${CMAKE_SOURCE_DIR}/3rdparty/eigen-3.4.0"
#             CACHE PATH
#             "Eigen3 root"
#         )

#     endif()



#     find_path(
#         EIGEN3_INCLUDE_DIR

#         NAMES Eigen/Core

#         PATHS

#             ${EIGEN3_ROOT}
#             ${EIGEN3_ROOT}/include
#             ${EIGEN3_ROOT}/include/eigen3
#             /usr/include/eigen3
#             /usr/local/include/eigen3
#     )



#     if(NOT EIGEN3_INCLUDE_DIR)

#         message(
#             FATAL_ERROR
#             "Eigen3 not found"
#         )

#     endif()



#     message(
#         STATUS
#         "Eigen3 include: ${EIGEN3_INCLUDE_DIR}"
#     )



#     if(NOT TARGET Eigen3::Eigen)


#         add_library(
#             Eigen3::Eigen
#             INTERFACE IMPORTED
#         )


#         set_target_properties(
#             Eigen3::Eigen

#             PROPERTIES

#             INTERFACE_INCLUDE_DIRECTORIES
#             "${EIGEN3_INCLUDE_DIR}"

#         )


#     endif()


# endif()



# # ============================================================
# # Python
# # ============================================================

# if(OPEN_CAX_BUILD_PYTHON)


#     find_package(
#         Python3
#         REQUIRED
#         COMPONENTS
#         Interpreter
#         Development
#     )


# endif()



# # ============================================================
# # MPI
# # ============================================================

# if(OPEN_CAX_SOLVER_USE_MPI)


#     find_package(
#         MPI
#         REQUIRED
#     )


#     message(
#         STATUS
#         "MPI enabled"
#     )


# endif()



# # ============================================================
# # MUMPS
# # ============================================================

# if(OPEN_CAX_SOLVER_USE_MUMPS)


#     find_package(
#         MUMPS
#         REQUIRED
#     )


#     message(
#         STATUS
#         "MUMPS enabled"
#     )


# endif()



# # ============================================================
# # PETSc
# # ============================================================

# if(OPEN_CAX_SOLVER_USE_PETSC)


#     find_package(
#         PETSc
#         REQUIRED
#     )


#     message(
#         STATUS
#         "PETSc enabled"
#     )


# endif()



# # ============================================================
# # CUDA Solver
# # ============================================================

# if(OPEN_CAX_SOLVER_USE_CUDA)


#     find_package(
#         CUDAToolkit
#         REQUIRED
#     )


#     message(
#         STATUS
#         "CUDA Toolkit ${CUDAToolkit_VERSION}"
#     )


# endif()



# # ============================================================
# # OpenMP
# # ============================================================

# if(OPEN_CAX_SOLVER_USE_OPENMP)


#     find_package(
#         OpenMP
#         REQUIRED
#     )


# endif()



# # ============================================================
# # METIS
# # ============================================================

# if(OPEN_CAX_SOLVER_USE_METIS)


#     find_package(
#         METIS
#         REQUIRED
#     )


# endif()

# ============================================================
# OpenCAX Third-party Dependencies
# ============================================================


include_guard(GLOBAL)



# ============================================================
# Helper Functions
# ============================================================


function(opencax_print_dependency name value)

    message(
        STATUS
        "${name}: ${value}"
    )

endfunction()



# ============================================================
# OpenCASCADE
# Geometry Kernel
# ============================================================


if(NOT OpenCASCADE_DIR)

    set(
        OpenCASCADE_DIR

        "${CMAKE_SOURCE_DIR}/3rdparty/occt-7.9.2/lib/cmake/opencascade"

        CACHE PATH
        "OpenCASCADE cmake directory"
    )

endif()



find_package(
    OpenCASCADE
    REQUIRED
)


opencax_print_dependency(
    "OpenCASCADE"
    "${OpenCASCADE_INSTALL_PREFIX}"
)



# ============================================================
# VTK
# Post Processing
# ============================================================


if(OPEN_CAX_BUILD_POST)


    if(NOT VTK_DIR)

        set(
            VTK_DIR

            "${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/lib/cmake/vtk-9.4"

            CACHE PATH
            "VTK cmake directory"
        )

    endif()


    find_package(
        VTK
        REQUIRED
    )


    opencax_print_dependency(
        "VTK"
        "${VTK_VERSION}"
    )


endif()



# ============================================================
# Gmsh
# ============================================================


if(
    OPEN_CAX_BUILD_MESH
    AND OPEN_CAX_MESH_USE_GMSH
)


    if(NOT GMSH_ROOT)

        set(
            GMSH_ROOT

            "${CMAKE_SOURCE_DIR}/3rdparty/gmsh-4.15"

            CACHE PATH
            "Gmsh root"
        )

    endif()



    find_path(
        GMSH_INCLUDE_DIR

        gmsh.h

        PATHS
        ${GMSH_ROOT}/include

        REQUIRED
    )



    find_library(
        GMSH_LIBRARY

        gmsh

        PATHS
        ${GMSH_ROOT}/lib

        REQUIRED
    )


    if(NOT TARGET Gmsh::Gmsh)

        add_library(
            Gmsh::Gmsh

            UNKNOWN IMPORTED
        )


        set_target_properties(
            Gmsh::Gmsh

            PROPERTIES

            IMPORTED_LOCATION
            ${GMSH_LIBRARY}

            INTERFACE_INCLUDE_DIRECTORIES
            ${GMSH_INCLUDE_DIR}
        )


    endif()


endif()



# ============================================================
# TetGen
# ============================================================


if(
    OPEN_CAX_BUILD_MESH
    AND OPEN_CAX_MESH_USE_TETGEN
)


    if(NOT TARGET TetGen::TetGen)


        add_library(
            OpenCAXTetGen

            ${CMAKE_SOURCE_DIR}/3rdparty/tetgen/tetgen.cxx
            ${CMAKE_SOURCE_DIR}/3rdparty/tetgen/predicates.cxx
        )


        add_library(
            TetGen::TetGen
            ALIAS
            OpenCAXTetGen
        )


        target_include_directories(
            OpenCAXTetGen

            PUBLIC

            ${CMAKE_SOURCE_DIR}/3rdparty/tetgen
        )


        target_compile_definitions(
            OpenCAXTetGen

            PUBLIC

            TETLIBRARY
        )


    endif()


endif()



# ============================================================
# GoogleTest
# ============================================================


if(OPEN_CAX_BUILD_TESTS)

    find_package(
        GTest
        REQUIRED
    )

endif()



# ============================================================
# spdlog
# ============================================================


if(OPEN_CAX_BUILD_LOGGING)


    if(NOT SPDLOG_ROOT)

        set(
            SPDLOG_ROOT

            "${CMAKE_SOURCE_DIR}/3rdparty/spdlog"

            CACHE PATH
            "spdlog root"
        )

    endif()


    add_library(
        spdlog::spdlog

        INTERFACE IMPORTED
    )


    set_target_properties(
        spdlog::spdlog

        PROPERTIES

        INTERFACE_INCLUDE_DIRECTORIES
        "${SPDLOG_ROOT}/include"

    )


endif()



# ============================================================
# Eigen
# ============================================================


if(
    OPEN_CAX_BUILD_FEM
    OR OPEN_CAX_SOLVER_USE_EIGEN
)


    if(NOT EIGEN3_ROOT)

        set(
            EIGEN3_ROOT

            "${CMAKE_SOURCE_DIR}/3rdparty/eigen-3.4.0"

            CACHE PATH
            "Eigen3 root"
        )

    endif()



    if(NOT TARGET Eigen3::Eigen)


        add_library(
            Eigen3::Eigen

            INTERFACE IMPORTED
        )


        set_target_properties(
            Eigen3::Eigen

            PROPERTIES

            INTERFACE_INCLUDE_DIRECTORIES

            ${EIGEN3_ROOT}

        )


    endif()


    opencax_print_dependency(
        "Eigen3"
        "${EIGEN3_ROOT}"
    )


endif()



# ============================================================
# MPI
# ============================================================


if(OPEN_CAX_SOLVER_USE_MPI)


    find_package(
        MPI
        REQUIRED
    )


    opencax_print_dependency(
        "MPI"
        "Enabled"
    )


endif()



# ============================================================
# MUMPS
# ============================================================


if(OPEN_CAX_SOLVER_USE_MUMPS)


    find_package(
        MUMPS
        REQUIRED
    )


endif()



# ============================================================
# PETSc
# ============================================================


if(OPEN_CAX_SOLVER_USE_PETSC)


    find_package(
        PETSc
        REQUIRED
    )


endif()



# ============================================================
# CUDA
# ============================================================


if(OPEN_CAX_SOLVER_USE_CUDA)


    find_package(
        CUDAToolkit
        REQUIRED
    )


    opencax_print_dependency(
        "CUDA"
        "${CUDAToolkit_VERSION}"
    )


endif()



# ============================================================
# OpenMP
# ============================================================


if(OPEN_CAX_SOLVER_USE_OPENMP)


    find_package(
        OpenMP
        REQUIRED
    )


endif()



# ============================================================
# METIS
# ============================================================


if(OPEN_CAX_SOLVER_USE_METIS)


    find_package(
        METIS
        REQUIRED
    )


endif()
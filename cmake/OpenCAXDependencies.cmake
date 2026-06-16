# ============================================================
# OpenCAX Third-party Dependencies
# ============================================================

# ==============================
# OpenCASCADE
# ==============================
if(NOT OpenCASCADE_DIR)
    set(OpenCASCADE_DIR
        "${CMAKE_SOURCE_DIR}/3rdparty/occt-7.9.2/lib/cmake/opencascade"
        CACHE PATH "OpenCASCADE CMake package directory"
    )
endif()

find_package(OpenCASCADE REQUIRED)

message(STATUS "OpenCASCADE include: ${OpenCASCADE_INCLUDE_DIR}")
message(STATUS "OpenCASCADE libs: ${OpenCASCADE_LIBRARIES}")
message(STATUS "OpenCASCADE prefix: ${OpenCASCADE_INSTALL_PREFIX}")


# ==============================
# VTK
# Used by Post/Viewer
# ==============================
if(OPEN_CAX_BUILD_POST)

    if(NOT VTK_DIR)
        set(VTK_DIR
            "${CMAKE_SOURCE_DIR}/3rdparty/vtk-9.4.2/lib/cmake/vtk-9.4"
            CACHE PATH "VTK CMake package directory"
        )
    endif()

    find_package(VTK REQUIRED)

    message(STATUS "VTK include: ${VTK_INCLUDE_DIRS}")
    message(STATUS "VTK libs: ${VTK_LIBRARIES}")

endif()


# ==============================
# Gmsh
# Used by Mesh/Gmsh
# ==============================
if(OPEN_CAX_BUILD_MESH)

    if(NOT GMSH_ROOT)
        set(GMSH_ROOT
            "${CMAKE_SOURCE_DIR}/3rdparty/gmsh-4.15"
            CACHE PATH "Gmsh root directory"
        )
    endif()

    find_path(GMSH_INCLUDE_DIR
        NAMES gmsh.h
        PATHS
            ${GMSH_ROOT}/include
        REQUIRED
    )

    find_library(GMSH_LIBRARY
        NAMES gmsh
        PATHS
            ${GMSH_ROOT}/lib
        REQUIRED
    )

    message(STATUS "Gmsh include: ${GMSH_INCLUDE_DIR}")
    message(STATUS "Gmsh lib: ${GMSH_LIBRARY}")

    if(NOT TARGET Gmsh::Gmsh)
        add_library(Gmsh::Gmsh UNKNOWN IMPORTED)

        set_target_properties(Gmsh::Gmsh PROPERTIES
            IMPORTED_LOCATION "${GMSH_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GMSH_INCLUDE_DIR}"
        )
    endif()

endif()


# ==============================
# TetGen
# Used by Mesh/TetGen
# ==============================
if(OPEN_CAX_BUILD_MESH)

    if(NOT TETGEN_ROOT)
        set(TETGEN_ROOT
            "${CMAKE_SOURCE_DIR}/3rdparty/tetgen"
            CACHE PATH "TetGen root directory"
        )
    endif()

    find_path(TETGEN_INCLUDE_DIR
        NAMES tetgen.h
        PATHS
            ${TETGEN_ROOT}
            ${TETGEN_ROOT}/include
        REQUIRED
    )

    find_file(TETGEN_SOURCE_FILE
        NAMES tetgen.cxx
        PATHS
            ${TETGEN_ROOT}
            ${TETGEN_ROOT}/src
        REQUIRED
    )

    find_file(TETGEN_PREDICATES_FILE
        NAMES predicates.cxx
        PATHS
            ${TETGEN_ROOT}
            ${TETGEN_ROOT}/src
        REQUIRED
    )

    message(STATUS "TetGen include: ${TETGEN_INCLUDE_DIR}")
    message(STATUS "TetGen source: ${TETGEN_SOURCE_FILE}")
    message(STATUS "TetGen predicates: ${TETGEN_PREDICATES_FILE}")

    if(NOT TARGET TetGen::TetGen)

        add_library(OpenCAXTetGen
            ${TETGEN_SOURCE_FILE}
            ${TETGEN_PREDICATES_FILE}
        )

        add_library(TetGen::TetGen ALIAS OpenCAXTetGen)

        target_include_directories(OpenCAXTetGen
            PUBLIC
                ${TETGEN_INCLUDE_DIR}
        )

        target_compile_definitions(OpenCAXTetGen
            PUBLIC
                TETLIBRARY
            PRIVATE
                TETGEN_EXPORTS
        )

        set_target_properties(OpenCAXTetGen PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            OUTPUT_NAME "tetgen"
        )

    endif()

endif()


# ==============================
# GoogleTest
# ==============================
if(OPEN_CAX_BUILD_TESTS)

    if(NOT GTEST_ROOT)
        set(GTEST_ROOT
            "${CMAKE_SOURCE_DIR}/3rdparty/googletest"
            CACHE PATH "GoogleTest root directory"
        )
    endif()

    find_path(GTEST_INCLUDE_DIR
        NAMES gtest/gtest.h
        PATHS
            ${GTEST_ROOT}/include
        REQUIRED
    )

    find_library(GTEST_LIBRARY
        NAMES gtest
        PATHS
            ${GTEST_ROOT}/lib
        REQUIRED
    )

    find_library(GTEST_MAIN_LIBRARY
        NAMES gtest_main
        PATHS
            ${GTEST_ROOT}/lib
        REQUIRED
    )

    message(STATUS "GoogleTest include: ${GTEST_INCLUDE_DIR}")
    message(STATUS "GoogleTest lib: ${GTEST_LIBRARY}")
    message(STATUS "GoogleTest main lib: ${GTEST_MAIN_LIBRARY}")

    if(NOT TARGET GTest::gtest)
        add_library(GTest::gtest UNKNOWN IMPORTED)
        set_target_properties(GTest::gtest PROPERTIES
            IMPORTED_LOCATION "${GTEST_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIR}"
        )
    endif()

    if(NOT TARGET GTest::gtest_main)
        add_library(GTest::gtest_main UNKNOWN IMPORTED)
        set_target_properties(GTest::gtest_main PROPERTIES
            IMPORTED_LOCATION "${GTEST_MAIN_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIR}"
        )
    endif()

endif()


# ==============================
# spdlog
# ==============================
if(OPEN_CAX_BUILD_LOGGING)

    if(NOT SPDLOG_ROOT)
        set(SPDLOG_ROOT
            "${CMAKE_SOURCE_DIR}/3rdparty/spdlog"
            CACHE PATH "spdlog root directory"
        )
    endif()

    find_path(SPDLOG_INCLUDE_DIR
        NAMES spdlog/spdlog.h
        PATHS
            ${SPDLOG_ROOT}/include
        REQUIRED
    )

    find_library(SPDLOG_LIBRARY
        NAMES spdlog spdlogd
        PATHS
            ${SPDLOG_ROOT}/lib
    )

    message(STATUS "spdlog include: ${SPDLOG_INCLUDE_DIR}")
    message(STATUS "spdlog lib: ${SPDLOG_LIBRARY}")

    if(SPDLOG_LIBRARY)
        if(NOT TARGET spdlog::spdlog)
            add_library(spdlog::spdlog UNKNOWN IMPORTED)

            set_target_properties(spdlog::spdlog PROPERTIES
                IMPORTED_LOCATION "${SPDLOG_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${SPDLOG_INCLUDE_DIR}"
            )
        endif()
    else()
        if(NOT TARGET spdlog::spdlog)
            add_library(spdlog::spdlog INTERFACE IMPORTED)

            set_target_properties(spdlog::spdlog PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${SPDLOG_INCLUDE_DIR}"
            )
        endif()
    endif()

endif()
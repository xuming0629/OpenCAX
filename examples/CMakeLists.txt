# ============================================================
# add_opencax_example
#
# Usage:
#   add_opencax_example(test_xxx test_xxx.cpp)
#
# This helper links examples with enabled OpenCAX modules.
# It supports both old Mesh and new Mesh2/Meshing during migration.
# ============================================================

function(add_opencax_example name source)

    add_executable(${name}
        ${source}
    )

    target_compile_features(${name}
        PRIVATE
            cxx_std_17
    )

    target_include_directories(${name}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )

    # ------------------------------------------------------------
    # Core / Geometry
    # ------------------------------------------------------------
    target_link_libraries(${name}
        PRIVATE
            OpenCAX::Core
    )

    if(TARGET OpenCAX::Geometry)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::Geometry
        )
    endif()

    # ------------------------------------------------------------
    # Old Mesh
    # ------------------------------------------------------------
    if(OPEN_CAX_BUILD_MESH AND TARGET OpenCAX::Mesh)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::Mesh
        )
    endif()



    # ------------------------------------------------------------
    # Meshing
    # ------------------------------------------------------------
    if(OPEN_CAX_BUILD_MESHING AND TARGET OpenCAX::Meshing)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::Meshing
        )
    endif()

    # ------------------------------------------------------------
    # Post Plot
    # ------------------------------------------------------------
    if(OPEN_CAX_BUILD_POST AND TARGET OpenCAX::PostPlot)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::PostPlot
        )
    endif()

    # ------------------------------------------------------------
    # Post Viewer
    # ------------------------------------------------------------
    if(OPEN_CAX_BUILD_POST AND TARGET OpenCAX::PostViewer)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::PostViewer
        )
    endif()

    # ------------------------------------------------------------
    # Post Recovery
    # ------------------------------------------------------------
    if(OPEN_CAX_BUILD_POST AND TARGET OpenCAX::PostRecovery)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::PostRecovery
        )
    endif()

    # ------------------------------------------------------------
    # FEM
    # ------------------------------------------------------------
    if(OPEN_CAX_BUILD_FEM AND TARGET OpenCAX::FEM)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::FEM
        )

        if(TARGET Eigen3::Eigen)
            target_link_libraries(${name}
                PRIVATE
                    Eigen3::Eigen
            )
        endif()
    endif()

    # ------------------------------------------------------------
    # Python
    # ------------------------------------------------------------
    if(OPEN_CAX_BUILD_PYTHON)
        if(DEFINED Python3_INCLUDE_DIRS)
            target_include_directories(${name}
                PRIVATE
                    ${Python3_INCLUDE_DIRS}
            )
        endif()

        if(TARGET Python3::Python)
            target_link_libraries(${name}
                PRIVATE
                    Python3::Python
            )
        endif()
    endif()

    # ------------------------------------------------------------
    # Gmsh
    # ------------------------------------------------------------
    if(OPEN_CAX_ENABLE_GMSH)
        if(TARGET Gmsh::Gmsh)
            target_link_libraries(${name}
                PRIVATE
                    Gmsh::Gmsh
            )
        endif()
    endif()

    # ------------------------------------------------------------
    # TetGen
    # ------------------------------------------------------------
    if(OPEN_CAX_ENABLE_TETGEN)
        if(TARGET TetGen::TetGen)
            target_link_libraries(${name}
                PRIVATE
                    TetGen::TetGen
            )
        endif()
    endif()

endfunction()





add_opencax_example(
    test_meshing_triangle6
    test_meshing_triangle6.cpp
)

add_opencax_example(
    test_meshing_triangle3
    test_meshing_triangle3.cpp
)

add_opencax_example(
    test_meshing_refine
    test_meshing_refine.cpp
)

add_opencax_example(
    test_meshing_quad4
    test_meshing_quad4.cpp
)

add_opencax_example(
    test_meshing_quda8
    test_meshing_quda8.cpp
)

add_opencax_example(
    test_meshing_tetra4
    test_meshing_tetra4.cpp
)

add_opencax_example(
    test_meshing_hex8
    test_meshing_hex8.cpp
)
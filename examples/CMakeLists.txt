function(add_opencax_example name source)
    add_executable(${name} ${source})

    target_link_libraries(${name}
        PRIVATE
            OpenCAX::Core
            OpenCAX::Geometry
    )

    if(OPEN_CAX_BUILD_MESH)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::Mesh
                OpenCAX::PostPlot
        )
    endif()

    if(OPEN_CAX_BUILD_FEM)
        target_link_libraries(${name}
            PRIVATE
                # OpenCAX::FEM
                Eigen3::Eigen
        
        )
    endif()

    if(OPEN_CAX_BUILD_POST)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::PostViewer
        )
    endif()

    if (OPEN_CAX_BUILD_PYTHON)
        target_include_directories(${name}
            PRIVATE
                ${Python3_INCLUDE_DIRS}
        )

        target_link_libraries(${name}
            PRIVATE
                Python3::Python
    
        )
    endif()
        
endfunction()

# add_opencax_example(
#     test_triangle_mesh
#     test_triangle_mesh.cpp
# )

# add_opencax_example(
#     test_poisson2d
#     test_poisson2d.cpp
# )

# add_opencax_example(
#     test_pyplot_mesh
#     test_pyplot_mesh.cpp
# )
add_opencax_example(
    test_triangle_mesh_structured
    test_triangle_mesh_structured.cpp
)


add_opencax_example(
    test_triangle_mesh_with_topology
    test_triangle_mesh_with_topology.cpp
)

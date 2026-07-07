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
                OpenCAX::FEM
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
# add_opencax_example(
#     test_triangle_mesh_structured
#     test_triangle_mesh_structured.cpp
# )


add_opencax_example(
    test_example_quad_mesh
    test_example_quad_mesh.cpp
)


add_opencax_example(
    test_example_triangle_mesh
    test_example_triangle_mesh.cpp
)

add_opencax_example(
    test_example_tetra_mesh
    test_example_tetra_mesh.cpp
)


add_opencax_example(
    test_example_hex_mesh
    test_example_hex_mesh.cpp
)

add_opencax_example(
    test_triangle_mesh_structured
    test_triangle_mesh_structured.cpp
)


add_opencax_example(
    test_gmsh_structured_triangle_mesh
    test_gmsh_structured_triangle_mesh.cpp
)

add_opencax_example(
    test_gmsh_unstructured_triangle_mesh
    test_gmsh_unstructured_triangle_mesh.cpp
)


add_opencax_example(
    test_triangle_mesh_bisection_refine
    test_triangle_mesh_bisection_refine.cpp
)

add_opencax_example(
    test_triangle_mesh_uniform_refine
    test_triangle_mesh_uniform_refine.cpp
)

add_opencax_example(
    test_poisson2d_p1
    test_poisson2d_p1.cpp
)

add_opencax_example(
    test_poisson2d_convergence
    test_poisson2d_convergence.cpp
)

add_opencax_example(
    test_poisson2d_convergence_uniform_refine
    test_poisson2d_convergence_uniform_refine.cpp
)

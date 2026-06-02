function(add_opencax_example name source)
    add_executable(${name} ${source})

    target_link_libraries(${name}
        PRIVATE
            OpenCAX::Core
            OpenCAX::Geometry
            OpenCAX::IO
    )

    if(OPEN_CAX_BUILD_MESHING)
        target_link_libraries(${name} PRIVATE OpenCAX::Meshing)
    endif()

    if(OPEN_CAX_BUILD_VISUALIZATION)
        target_link_libraries(${name} PRIVATE OpenCAX::Visualization)
    endif()
endfunction()



add_opencax_example(
    example_line_mesh
    example_line_mesh.cpp
)

add_opencax_example(
    example_tri_mesh
    example_tri_mesh.cpp
)

add_opencax_example(
    example_quad_mesh
    example_quad_mesh.cpp
)



add_opencax_example(
    example_hex_mesh
    example_hex_mesh.cpp
)

add_opencax_example(
    example_tet_mesh
    example_tet_mesh.cpp
)

add_opencax_example(
    example_logger
    example_logger.cpp
)


add_opencax_example(
    example_gmsh_tet_mesh
    example_gmsh_tet_mesh.cpp
)


add_opencax_example(
    example_gmsh_hex_mesh
    example_gmsh_hex_mesh.cpp
)

add_opencax_example(
    example_mesh_plane
    example_mesh_plane.cpp
)



add_opencax_example(
    example_mesh_triangle
    example_mesh_triangle.cpp
)

add_opencax_example(
    example_mesh_polygon
    example_mesh_polygon.cpp
)


add_opencax_example(
    example_mesh_curved_surface
    example_mesh_curved_surface.cpp
)


add_opencax_example(
    example_mesh_tetra
    example_mesh_tetra.cpp)

add_opencax_example(
    example_mesh_hexa
    example_mesh_hexa.cpp)

add_opencax_example(
    example_plane_quad
    example_plane_quad.cpp)

add_opencax_example(
    example_surface_quad
    example_surface_quad.cpp)

add_opencax_example(
    example_plane_tri_quad_mixed
    example_plane_tri_quad_mixed.cpp)
   
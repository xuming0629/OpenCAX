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

# add_opencax_example(
#     example_box
#     example_box.cpp
# )

add_opencax_example(
    example_line_mesh
    example_line_mesh.cpp
)

# add_opencax_example(
#     example_tri_mesh
#     example_tri_mesh.cpp
# )

add_opencax_example(
    example_quad_mesh
    example_quad_mesh.cpp
)

# add_opencax_example(
#     example_tet_mesh
#     example_tet_mesh.cpp
# )

# add_opencax_example(
#     example_hex_mesh
#     example_hex_mesh.cpp
# )
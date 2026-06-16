function(add_opencax_example name source)
    add_executable(${name} ${source})

    target_link_libraries(${name}
        PRIVATE
            OpenCAX::Core
            OpenCAX::Geometry
            # OpenCAX::IO
    )

    if(OPEN_CAX_BUILD_MESH)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::Mesh
        )
    endif()

    if(OPEN_CAX_BUILD_POST)
        target_link_libraries(${name}
            PRIVATE
                OpenCAX::PostViewer
        )
    endif()
endfunction()



add_opencax_example(
    test_triangle_mesh
    test_triangle_mesh.cpp
)



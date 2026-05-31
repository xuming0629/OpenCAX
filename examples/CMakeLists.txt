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

add_opencax_example(example_box example_box.cpp)
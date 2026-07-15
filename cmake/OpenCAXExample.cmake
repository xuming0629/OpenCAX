# ============================================================
# OpenCAX Example Helper
#
# Usage:
#   add_opencax_example(example_name source.cpp OpenCAX::Mesh)
# ============================================================

include_guard(GLOBAL)

function(add_opencax_example name source)
    if(TARGET ${name})
        message(FATAL_ERROR "Target '${name}' already exists")
    endif()

    add_executable(${name} ${source})

    target_compile_features(${name} PRIVATE cxx_std_17)
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${name} PRIVATE OpenCAX::Core)

    foreach(lib IN LISTS ARGN)
        if(TARGET ${lib})
            target_link_libraries(${name} PRIVATE ${lib})
        else()
            message(FATAL_ERROR "add_opencax_example(${name}): target '${lib}' does not exist")
        endif()
    endforeach()

    set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endfunction()
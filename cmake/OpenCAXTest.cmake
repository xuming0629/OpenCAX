# ============================================================
# OpenCAX Test Helper
# ============================================================

include_guard(GLOBAL)
include(GoogleTest)

function(add_opencax_test name source)

    if(TARGET ${name})
        message(FATAL_ERROR "Target '${name}' already exists")
    endif()

    add_executable(${name} ${source})

    target_compile_features(${name} PRIVATE cxx_std_17)
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${name} PRIVATE GTest::gtest GTest::gtest_main OpenCAX::Core)

    foreach(lib IN LISTS ARGN)
        if(TARGET ${lib})
            target_link_libraries(${name} PRIVATE ${lib})
        else()
            message(FATAL_ERROR "add_opencax_test(${name}): target '${lib}' does not exist")
        endif()
    endforeach()

    if(COMMAND gtest_discover_tests)
        gtest_discover_tests(${name} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} DISCOVERY_TIMEOUT 30)
    else()
        add_test(NAME ${name} COMMAND ${name})
    endif()

endfunction()
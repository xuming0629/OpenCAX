add_library(OpenCAXCore
    Logger.cpp
)

add_library(OpenCAX::Core ALIAS OpenCAXCore)

target_include_directories(OpenCAXCore
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${SPDLOG_INCLUDE_DIR}
)

target_compile_features(OpenCAXCore
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXCore
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXCore
    PUBLIC
        ${SPDLOG_LIBRARY}
)
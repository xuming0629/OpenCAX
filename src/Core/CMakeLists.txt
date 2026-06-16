# add_library(OpenCAXCore
#     Logger.cpp
# )

# add_library(OpenCAX::Core ALIAS OpenCAXCore)

# target_include_directories(OpenCAXCore
#     PUBLIC
#         ${CMAKE_SOURCE_DIR}/include
#         ${SPDLOG_INCLUDE_DIR}
# )

# target_compile_features(OpenCAXCore
#     PUBLIC
#         cxx_std_17
# )

# target_compile_definitions(OpenCAXCore
#     PRIVATE
#         OpenCAX_EXPORTS
# )

# target_link_libraries(OpenCAXCore
#     PUBLIC
#         ${SPDLOG_LIBRARY}
# )

add_library(OpenCAXCore
    Logger.cpp
)

add_library(OpenCAX::Core ALIAS OpenCAXCore)

target_include_directories(OpenCAXCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
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
        spdlog::spdlog
)

set_target_properties(OpenCAXCore PROPERTIES
    OUTPUT_NAME OpenCAXCore
    EXPORT_NAME Core
)

install(TARGETS OpenCAXCore
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/OpenCAX/Core
    DESTINATION include/OpenCAX
)
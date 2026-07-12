add_library(OpenCAXMeshingRefinement
    MeshRefiner.cpp
)

add_library(OpenCAX::MeshingRefinement ALIAS OpenCAXMeshingRefinement)

target_include_directories(OpenCAXMeshingRefinement
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXMeshingRefinement
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXMeshingRefinement
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXMeshingRefinement
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
)

set_target_properties(OpenCAXMeshingRefinement PROPERTIES
    OUTPUT_NAME OpenCAXMeshingRefinement
    EXPORT_NAME MeshingRefinement
)

install(TARGETS OpenCAXMeshingRefinement
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

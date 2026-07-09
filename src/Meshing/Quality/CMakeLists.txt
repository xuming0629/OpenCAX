# ============================================================
# OpenCAX Meshing Quality
#
# Mesh quality metrics and validation helpers.
# ============================================================

add_library(OpenCAXMeshingQuality
    MeshQuality.cpp
)

add_library(OpenCAX::MeshingQuality ALIAS OpenCAXMeshingQuality)

target_include_directories(OpenCAXMeshingQuality
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXMeshingQuality
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXMeshingQuality
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXMeshingQuality
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh2
)

set_target_properties(OpenCAXMeshingQuality PROPERTIES
    OUTPUT_NAME OpenCAXMeshingQuality
    EXPORT_NAME MeshingQuality
)

install(TARGETS OpenCAXMeshingQuality
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

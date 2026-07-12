# ============================================================
# OpenCAX Meshing Structured
#
# Structured generators:
#   rectangle_tri3 / rectangle_tri6
#   rectangle_quad4 / rectangle_quad8 / rectangle_quad9
#   box_tetra4 / box_tetra10
#   box_hexa8 / box_hexa20 / box_hexa27
# ============================================================

add_library(OpenCAXMeshingStructured
    StructuredMeshGenerator.cpp
)

add_library(OpenCAX::MeshingStructured ALIAS OpenCAXMeshingStructured)

target_include_directories(OpenCAXMeshingStructured
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXMeshingStructured
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXMeshingStructured
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXMeshingStructured
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
        OpenCAX::MeshingHighOrder
)

set_target_properties(OpenCAXMeshingStructured PROPERTIES
    OUTPUT_NAME OpenCAXMeshingStructured
    EXPORT_NAME MeshingStructured
)

install(TARGETS OpenCAXMeshingStructured
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

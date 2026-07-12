# ============================================================
# OpenCAX Meshing HighOrder
#
# High-order mesh conversion:
#   Triangle3 -> Triangle6
#   Quad4     -> Quad8 / Quad9
#   Tetra4    -> Tetra10
#   Hexa8     -> Hexa20 / Hexa27
# ============================================================

add_library(OpenCAXMeshingHighOrder
    MeshOrderConverter.cpp
)

add_library(OpenCAX::MeshingHighOrder ALIAS OpenCAXMeshingHighOrder)

target_include_directories(OpenCAXMeshingHighOrder
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXMeshingHighOrder
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXMeshingHighOrder
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXMeshingHighOrder
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
)

set_target_properties(OpenCAXMeshingHighOrder PROPERTIES
    OUTPUT_NAME OpenCAXMeshingHighOrder
    EXPORT_NAME MeshingHighOrder
)

install(TARGETS OpenCAXMeshingHighOrder
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

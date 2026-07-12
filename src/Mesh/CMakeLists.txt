# ============================================================
# OpenCAX Mesh
# Data structures + topology only.
#
# Temporary new mesh module used during migration.
# Later it can be renamed back to OpenCAXMesh / OpenCAX::Mesh.
# ============================================================

add_library(OpenCAXMesh
    Mesh.cpp
    CellTopology.cpp
    MeshTopology.cpp
    TriangleMesh.cpp
    QuadMesh.cpp
    TetraMesh.cpp
    HexMesh.cpp
    MeshDebugUtils.cpp
)

add_library(OpenCAX::Mesh ALIAS OpenCAXMesh)

target_include_directories(OpenCAXMesh
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXMesh
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXMesh
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXMesh
    PUBLIC
        OpenCAX::Core
)

set_target_properties(OpenCAXMesh PROPERTIES
    OUTPUT_NAME OpenCAXMesh
    EXPORT_NAME Mesh
)

install(TARGETS OpenCAXMesh
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/OpenCAX/Mesh
    DESTINATION include/OpenCAX
)
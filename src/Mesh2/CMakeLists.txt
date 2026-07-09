# ============================================================
# OpenCAX Mesh2
# Data structures + topology only.
#
# Temporary new mesh module used during migration.
# Later it can be renamed back to OpenCAXMesh / OpenCAX::Mesh.
# ============================================================

add_library(OpenCAXMesh2
    Mesh.cpp
    CellTopology.cpp
    MeshTopology.cpp
    TriangleMesh.cpp
    QuadMesh.cpp
    TetraMesh.cpp
    HexMesh.cpp
    MeshDebugUtils.cpp
)

add_library(OpenCAX::Mesh2 ALIAS OpenCAXMesh2)

target_include_directories(OpenCAXMesh2
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXMesh2
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXMesh2
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXMesh2
    PUBLIC
        OpenCAX::Core
)

set_target_properties(OpenCAXMesh2 PROPERTIES
    OUTPUT_NAME OpenCAXMesh2
    EXPORT_NAME Mesh2
)

install(TARGETS OpenCAXMesh2
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/OpenCAX/Mesh2
    DESTINATION include/OpenCAX
)
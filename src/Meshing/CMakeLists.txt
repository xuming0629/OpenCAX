add_library(OpenCAXMesh
    Mesh.cpp
    StructuredMeshGenerator.cpp
    GmshMesher.cpp
    TetgenMesher.cpp
)

add_library(OpenCAX::Meshing ALIAS OpenCAXMesh)

target_include_directories(OpenCAXMesh
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${OpenCASCADE_INCLUDE_DIR}
        ${GMSH_INCLUDE_DIR}
        ${TETGEN_INCLUDE_DIR}
)

target_link_libraries(OpenCAXMesh
    PUBLIC
        OpenCAX::Core
        OpenCAX::Geometry
        OpenCAX::IO
        ${OpenCASCADE_LIBRARIES}
        ${GMSH_LIBRARY}
        tetgen
)
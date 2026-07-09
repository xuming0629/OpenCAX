
add_library(OpenCAXMesh
    Mesh.cpp
    CellTopology.cpp
    MeshTopology.cpp
    TriangleMesh.cpp
    QuadMesh.cpp
    TetraMesh.cpp
    HexMesh.cpp
    GmshTriangleMesh.cpp
    MeshDebugUtils.cpp
    TriangleMeshRefiner.cpp
    TriangleMeshOrderConverter.cpp

  
)

add_library(OpenCAX::Mesh ALIAS OpenCAXMesh)

target_include_directories(OpenCAXMesh
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${OpenCASCADE_INCLUDE_DIR}
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
        OpenCAX::Geometry
        ${OpenCASCADE_LIBRARIES}
)

if(OPEN_CAX_ENABLE_GMSH)
    target_include_directories(OpenCAXMesh
        PUBLIC
            ${GMSH_INCLUDE_DIR}
    )

    target_link_libraries(OpenCAXMesh
        PUBLIC
            Gmsh::Gmsh
    )

    target_compile_definitions(OpenCAXMesh
        PUBLIC
            OPEN_CAX_HAS_GMSH
    )
endif()

if(OPEN_CAX_ENABLE_TETGEN)
    target_include_directories(OpenCAXMesh
        PUBLIC
            ${TETGEN_INCLUDE_DIR}
    )

    target_link_libraries(OpenCAXMesh
        PUBLIC
            TetGen::TetGen
    )

    target_compile_definitions(OpenCAXMesh
        PUBLIC
            OPEN_CAX_HAS_TETGEN
    )
endif()

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

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/OpenCAX/Mesh
    DESTINATION include/OpenCAX
)
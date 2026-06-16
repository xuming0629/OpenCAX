add_library(OpenCAXMesh
    Mesh.cpp
    MeshTopology.cpp
    TriangleMesh.cpp
    # TetraMesh.cpp

)

add_library(OpenCAX::Mesh ALIAS OpenCAXMesh)

target_include_directories(OpenCAXMesh
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${OpenCASCADE_INCLUDE_DIR}
)

target_link_libraries(OpenCAXMesh
    PUBLIC
        OpenCAX::Core
        OpenCAX::Geometry
        # OpenCAX::IO
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
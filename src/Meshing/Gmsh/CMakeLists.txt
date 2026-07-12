# ============================================================
# OpenCAX Meshing Gmsh
#
# Gmsh backend.
# This target is always created so users can link OpenCAX::Meshing
# even when Gmsh is disabled. When OPEN_CAX_ENABLE_GMSH=OFF,
# Gmsh functions compile as stubs and throw runtime errors.
#
# To enable real Gmsh support, configure with:
#   -DOPEN_CAX_ENABLE_GMSH=ON
# and make sure target Gmsh::Gmsh exists.
# ============================================================

add_library(OpenCAXMeshingGmsh
    GmshElementMap.cpp
    GmshMeshReader.cpp
    GmshMeshGenerator.cpp
)

add_library(OpenCAX::MeshingGmsh ALIAS OpenCAXMeshingGmsh)

target_include_directories(OpenCAXMeshingGmsh
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXMeshingGmsh
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXMeshingGmsh
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXMeshingGmsh
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
        OpenCAX::MeshingHighOrder
)

if(OPEN_CAX_ENABLE_GMSH)
    if(NOT TARGET Gmsh::Gmsh)
        message(FATAL_ERROR "OPEN_CAX_ENABLE_GMSH=ON but target Gmsh::Gmsh was not found. Please call find_package(Gmsh REQUIRED) before add_subdirectory(src/Meshing), or define Gmsh::Gmsh yourself.")
    endif()

    target_compile_definitions(OpenCAXMeshingGmsh
        PUBLIC
            OPEN_CAX_HAS_GMSH
    )

    target_link_libraries(OpenCAXMeshingGmsh
        PUBLIC
            Gmsh::Gmsh
    )
endif()

set_target_properties(OpenCAXMeshingGmsh PROPERTIES
    OUTPUT_NAME OpenCAXMeshingGmsh
    EXPORT_NAME MeshingGmsh
)

install(TARGETS OpenCAXMeshingGmsh
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

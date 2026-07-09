# ============================================================
# OpenCAX Meshing
#
# Mesh generation layer.
# This module is intentionally split into small sub-targets:
#
#   OpenCAX::MeshingStructured
#   OpenCAX::MeshingHighOrder
#   OpenCAX::MeshingRefinement
#   OpenCAX::MeshingQuality
#   OpenCAX::MeshingGmsh
#
# And one aggregate target:
#
#   OpenCAX::Meshing
#
# Usage:
#   target_link_libraries(my_app PRIVATE OpenCAX::Meshing)
#
# Or link only what you need:
#   target_link_libraries(my_app PRIVATE OpenCAX::MeshingStructured)
# ============================================================

add_subdirectory(Structured)
add_subdirectory(HighOrder)
add_subdirectory(Refinement)
add_subdirectory(Quality)
add_subdirectory(Gmsh)

# ------------------------------------------------------------
# Aggregate interface target
# ------------------------------------------------------------
add_library(OpenCAXMeshing INTERFACE)
add_library(OpenCAX::Meshing ALIAS OpenCAXMeshing)

target_link_libraries(OpenCAXMeshing
    INTERFACE
        OpenCAX::MeshingStructured
        OpenCAX::MeshingHighOrder
        OpenCAX::MeshingRefinement
        OpenCAX::MeshingQuality
        OpenCAX::MeshingGmsh
)

target_include_directories(OpenCAXMeshing
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(OpenCAXMeshing PROPERTIES
    EXPORT_NAME Meshing
)

install(TARGETS OpenCAXMeshing
    EXPORT OpenCAXTargets
)

# Install the whole public Meshing include tree once here.
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/OpenCAX/Meshing
    DESTINATION include/OpenCAX
)

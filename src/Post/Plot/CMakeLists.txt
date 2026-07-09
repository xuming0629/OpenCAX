# ============================================================
# OpenCAX Post Plot
# ============================================================

add_library(OpenCAXPostPlot
    MeshPlotter.cpp
    ConvergencePlotter.cpp
)

add_library(OpenCAX::PostPlot ALIAS OpenCAXPostPlot)

target_include_directories(OpenCAXPostPlot
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXPostPlot
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXPostPlot
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXPostPlot
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
        OpenCAX::FEM
)

set_target_properties(OpenCAXPostPlot PROPERTIES
    OUTPUT_NAME OpenCAXPostPlot
    EXPORT_NAME PostPlot
)

install(TARGETS OpenCAXPostPlot
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/OpenCAX/Post/Plot
    DESTINATION include/OpenCAX/Post
)
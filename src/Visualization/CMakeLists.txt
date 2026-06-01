add_library(OpenCAXVisualization
    VtkViewer.cpp
    VisualOptions.cpp
    VtkConverter.cpp

)

add_library(OpenCAX::Visualization ALIAS OpenCAXVisualization)

target_include_directories(OpenCAXVisualization
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${VTK_INCLUDE_DIRS}
)

target_link_libraries(OpenCAXVisualization
    PUBLIC
        OpenCAX::Core
        OpenCAX::Meshing
        ${VTK_LIBRARIES}
)

vtk_module_autoinit(
    TARGETS OpenCAXVisualization
    MODULES ${VTK_LIBRARIES}
)
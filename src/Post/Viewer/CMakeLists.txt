add_library(OpenCAXPostViewer
    VtkViewer.cpp
    VtkConverter.cpp
    ScalarFieldViewer.cpp
    ConvergencePlotter.cpp
)

add_library(OpenCAX::PostViewer ALIAS OpenCAXPostViewer)

target_include_directories(OpenCAXPostViewer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(OpenCAXPostViewer
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
        Eigen3::Eigen
        ${VTK_LIBRARIES}
)

vtk_module_autoinit(
    TARGETS OpenCAXPostViewer
    MODULES ${VTK_LIBRARIES}
)
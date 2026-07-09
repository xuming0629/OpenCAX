# ============================================================
# OpenCAX Post Viewer
# ============================================================

add_library(OpenCAXPostViewer
    VtkViewer.cpp
    ScalarFieldViewer.cpp
    VtkConverter.cpp
    VectorFieldViewer.cpp
)

add_library(OpenCAX::PostViewer ALIAS OpenCAXPostViewer)

target_include_directories(OpenCAXPostViewer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OpenCAXPostViewer
    PUBLIC
        cxx_std_17
)

target_compile_definitions(OpenCAXPostViewer
    PRIVATE
        OpenCAX_EXPORTS
)

target_link_libraries(OpenCAXPostViewer
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
        Eigen3::Eigen
        ${VTK_LIBRARIES}
)

set_target_properties(OpenCAXPostViewer PROPERTIES
    OUTPUT_NAME OpenCAXPostViewer
    EXPORT_NAME PostViewer
)

vtk_module_autoinit(
    TARGETS OpenCAXPostViewer
    MODULES ${VTK_LIBRARIES}
)

install(TARGETS OpenCAXPostViewer
    EXPORT OpenCAXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/OpenCAX/Post/Viewer
    DESTINATION include/OpenCAX/Post
)
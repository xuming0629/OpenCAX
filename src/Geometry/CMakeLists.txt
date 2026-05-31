add_library(OpenCAXGeometry
    ShapeFactory.cpp
)

add_library(OpenCAX::Geometry ALIAS OpenCAXGeometry)

target_include_directories(OpenCAXGeometry
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${OpenCASCADE_INCLUDE_DIR}
)

target_link_libraries(OpenCAXGeometry
    PUBLIC
        OpenCAX::Core
        ${OpenCASCADE_LIBRARIES}
)
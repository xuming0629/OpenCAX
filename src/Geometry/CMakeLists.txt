add_library(OpenCAXGeometry
    Shape.cpp
    # GeometryBuilder.cpp
    # GeometryOps.cpp
    # GeometryAnalysis.cpp
    # GeometryIO.cpp
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
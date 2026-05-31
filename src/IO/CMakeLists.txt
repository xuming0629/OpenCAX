add_library(OpenCAXIO
    OccIO.cpp
)

add_library(OpenCAX::IO ALIAS OpenCAXIO)

target_include_directories(OpenCAXIO
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${OpenCASCADE_INCLUDE_DIR}
)

target_link_libraries(OpenCAXIO
    PUBLIC
        OpenCAX::Core
        OpenCAX::Geometry
        ${OpenCASCADE_LIBRARIES}
)
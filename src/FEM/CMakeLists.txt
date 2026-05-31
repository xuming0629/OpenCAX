add_library(OpenCAXFEM
    FEMModel.cpp
)

add_library(OpenCAX::FEM ALIAS OpenCAXFEM)

target_include_directories(OpenCAXFEM
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(OpenCAXFEM
    PUBLIC
        OpenCAX::Core
        OpenCAX::Meshing
)
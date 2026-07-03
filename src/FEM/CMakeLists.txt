# add_library(OpenCAXFEM
#     FEMModel.cpp
#     Poisson2D.cpp
# )

# add_library(OpenCAX::FEM ALIAS OpenCAXFEM)

# target_include_directories(OpenCAXFEM
#     PUBLIC
#         ${CMAKE_SOURCE_DIR}/include
# )

target_link_libraries(OpenCAXFEM
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
)

if(OPEN_CAX_USE_EIGEN3)
    target_link_libraries(OpenCAXFEM
        PRIVATE
            Eigen3::Eigen
    )
else()
    message(FATAL_ERROR "OpenCAX FEM requires Eigen3. Please enable OPEN_CAX_USE_EIGEN3=ON.")
endif()

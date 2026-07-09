add_library(OpenCAXFEM
    P1TriangleSpace.cpp
    SparseLUSolver.cpp
    CGSolver.cpp
    BiCGSTABSolver.cpp
    Material.cpp
    BoundaryManager.cpp
    TriangleQuadrature.cpp
    TriangleP1Element.cpp
    TriangleP2Element.cpp
    PhysicsProblem2D.cpp
    Poisson2D.cpp
    ScalarDiffusion2D.cpp
    HeatEquation2D.cpp
    ElasticMaterial2D.cpp
    Elasticity2D.cpp
    ErrorNorms2D.cpp
    ConvergenceStudy2D.cpp
)

add_library(OpenCAX::FEM ALIAS OpenCAXFEM)

target_include_directories(OpenCAXFEM
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(OpenCAXFEM
    PUBLIC
        OpenCAX::Core
        OpenCAX::Mesh
        OpenCAX::PostViewer

)

if(OPEN_CAX_USE_EIGEN3)
    target_link_libraries(OpenCAXFEM
        PRIVATE
            Eigen3::Eigen
    )
else()
    message(FATAL_ERROR "OpenCAX FEM requires Eigen3. Please enable OPEN_CAX_USE_EIGEN3=ON.")
endif()

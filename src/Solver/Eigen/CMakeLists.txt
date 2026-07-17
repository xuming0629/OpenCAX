add_library(
    OpenCAXSolverEigen
    EigenBiCGSTABSolver.cpp
    EigenCGSolver.cpp
    EigenSparseLUSolver.cpp
)


add_library(
    OpenCAX::SolverEigen

    ALIAS

    OpenCAXSolverEigen
)


target_include_directories(
    OpenCAXSolverEigen

    PUBLIC

    ${PROJECT_SOURCE_DIR}/include
)


target_link_libraries(
    OpenCAXSolverEigen

    PUBLIC

    Eigen3::Eigen
)
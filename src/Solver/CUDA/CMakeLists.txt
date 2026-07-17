# ============================================================
# CUDA Solver Backend
# ============================================================


enable_language(CUDA)



add_library(
    OpenCAXSolverCUDA

    CudaSolver.cpp

)



add_library(
    OpenCAX::SolverCUDA

    ALIAS

    OpenCAXSolverCUDA

)



target_include_directories(
    OpenCAXSolverCUDA

    PUBLIC

    ${PROJECT_SOURCE_DIR}/include

)



target_link_libraries(
    OpenCAXSolverCUDA

    PUBLIC

    CUDA::cudart

    CUDA::cusparse

    CUDA::cusolver

)



target_compile_definitions(
    OpenCAXSolverCUDA

    PUBLIC

    OPEN_CAX_HAS_CUDA

)
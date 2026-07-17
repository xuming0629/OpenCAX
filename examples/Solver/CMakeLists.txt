# ============================================================
# OpenCAX Solver Examples
# ============================================================


# ==========================
# Eigen Solver
# ==========================

if(OPEN_CAX_SOLVER_USE_EIGEN)

    add_opencax_example(
        test_solver_eigen
        test_solver_eigen.cpp
        OpenCAX::Solver
    )

endif()



# # ==========================
# # MUMPS Solver
# # ==========================

# if(OPEN_CAX_SOLVER_USE_MUMPS)

#     add_opencax_example(
#         test_solver_mumps
#         test_solver_mumps.cpp
#         OpenCAX::Solver
#     )

# endif()



# # ==========================
# # PETSc Solver
# # ==========================

# if(OPEN_CAX_SOLVER_USE_PETSC)

#     add_opencax_example(
#         test_solver_petsc
#         test_solver_petsc.cpp
#         OpenCAX::Solver
#     )

# endif()



# # ==========================
# # CUDA Solver
# # ==========================

# if(OPEN_CAX_SOLVER_USE_CUDA)

#     add_opencax_example(
#         test_solver_cuda
#         test_solver_cuda.cpp
#         OpenCAX::Solver
#     )

# endif()
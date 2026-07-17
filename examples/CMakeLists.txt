# ============================================================
# OpenCAX Examples
# ============================================================

include(OpenCAXExample)



# ==========================
# Mesh
# ==========================

if(OPEN_CAX_BUILD_MESH)

    add_subdirectory(Mesh)

endif()



# ==========================
# Solver
# ==========================

if(OPEN_CAX_BUILD_SOLVER)

    add_subdirectory(Solver)

endif()



# ==========================
# FEM
# ==========================

# if(OPEN_CAX_BUILD_FEM)

#     add_subdirectory(FEM)

# endif()
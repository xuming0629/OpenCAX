# ============================================================
# OpenCAX Tests
# ============================================================

# find_package(GTest REQUIRED)

# include(GoogleTest)
include(OpenCAXTest)

# ============================================================
# Module Tests
# ============================================================

if(OPEN_CAX_BUILD_MESH)
    add_subdirectory(Mesh)
endif()

# if(OPEN_CAX_BUILD_MESHING)
#     add_subdirectory(Meshing)
# endif()

if(OPEN_CAX_BUILD_SOLVER)
    add_subdirectory(Solver)
endif()

# if(OPEN_CAX_BUILD_FEM)
#     add_subdirectory(FEM)
# endif()
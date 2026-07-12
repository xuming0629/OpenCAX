add_subdirectory(Core)
add_subdirectory(Geometry)

# ==========================
# Mesh
# ==========================
if(OPEN_CAX_BUILD_MESH)
    add_subdirectory(Mesh)
endif()

# ==========================
# Meshing
# ==========================
if(OPEN_CAX_BUILD_MESHING)
    add_subdirectory(Meshing)
endif()

# ==========================
# Post
# ==========================
if(OPEN_CAX_BUILD_POST)
    add_subdirectory(Post)
endif()

# ==========================
# FEM
# ==========================
if(OPEN_CAX_BUILD_FEM)
    add_subdirectory(FEM)
endif()
add_subdirectory(Core)
add_subdirectory(Geometry)

# ==========================
# Old Mesh
# ==========================
if(OPEN_CAX_BUILD_MESH)
    add_subdirectory(Mesh)
endif()

# ==========================
# New Mesh2
# ==========================
if(OPEN_CAX_BUILD_MESH2)
    add_subdirectory(Mesh2)
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
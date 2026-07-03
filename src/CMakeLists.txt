add_subdirectory(Core)
add_subdirectory(Geometry)
# add_subdirectory(IO)

# ==========================
# Mesh
# ==========================
if(OPEN_CAX_BUILD_MESH)
    add_subdirectory(Mesh)
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
# if(OPEN_CAX_BUILD_FEM)
#     add_subdirectory(FEM)
# endif()


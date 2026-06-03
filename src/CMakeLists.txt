add_subdirectory(Core)
add_subdirectory(Geometry)
add_subdirectory(IO)

if(OPEN_CAX_BUILD_MESHING)
    add_subdirectory(Meshing)
endif()

if(OPEN_CAX_BUILD_VISUALIZATION)
    add_subdirectory(Visualization)
endif()

# if(OPEN_CAX_BUILD_FEM)
#     add_subdirectory(FEM)
# endif()
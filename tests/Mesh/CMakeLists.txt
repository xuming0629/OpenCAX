# ============================================================
# OpenCAX Mesh Tests
# ============================================================

add_opencax_test(
        test_mesh_basic 
        test_mesh_basic.cpp 
        OpenCAX::Mesh
        OpenCAX::Meshing
        OpenCAX::PostViewer
    )

# add_opencax_test(test_mesh_cell_topology test_mesh_cell_topology.cpp OpenCAX::Mesh)

# add_opencax_test(test_mesh_topology_triangle3 test_mesh_topology_triangle3.cpp OpenCAX::Mesh)

# add_opencax_test(test_mesh_topology_triangle6 test_mesh_topology_triangle6.cpp OpenCAX::Mesh)

# add_opencax_test(test_mesh_topology_quad4 test_mesh_topology_quad4.cpp OpenCAX::Mesh)

# add_opencax_test(test_mesh_topology_quad8 test_mesh_topology_quad8.cpp OpenCAX::Mesh)

# add_opencax_test(test_mesh_topology_tetra4 test_mesh_topology_tetra4.cpp OpenCAX::Mesh)

# add_opencax_test(test_mesh_topology_tetra10 test_mesh_topology_tetra10.cpp OpenCAX::Mesh)

# add_opencax_test(test_mesh_topology_hexa8 test_mesh_topology_hexa8.cpp OpenCAX::Mesh)
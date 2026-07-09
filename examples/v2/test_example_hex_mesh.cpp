#include <OpenCAX/Mesh/HexMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Post/Viewer/VtkViewer.h>

#include <iostream>
#include <vector>

/* =========================
 * 打印 vector
 * ========================= */
template <typename T>
static void print_vector(const std::vector<T>& v)
{
    std::cout << "[";

    for (std::size_t i = 0; i < v.size(); ++i)
    {
        std::cout << v[i];

        if (i + 1 < v.size())
        {
            std::cout << ", ";
        }
    }

    std::cout << "]";
}

/* =========================
 * 打印 array<double, 3>
 * ========================= */
static void print_array3(
    const std::array<double, 3>& a
)
{
    std::cout << "["
              << a[0] << ", "
              << a[1] << ", "
              << a[2] << "]";
}

/* =========================
 * 打印 array<double, 6>
 * ========================= */
static void print_array6(
    const std::array<double, 6>& a
)
{
    std::cout << "["
              << a[0] << ", "
              << a[1] << ", "
              << a[2] << ", "
              << a[3] << ", "
              << a[4] << ", "
              << a[5] << "]";
}

/* =========================
 * 打印拓扑关系
 * ========================= */
static void print_topology(
    const OpenCAX::MeshTopology& topo
)
{
    std::cout << "\n===== TOPOLOGY =====\n";

    std::cout << "edges = " << topo.num_edges() << "\n";
    std::cout << "faces = " << topo.num_faces() << "\n";

    std::cout << "\n--- edges ---\n";
    for (std::size_t i = 0; i < topo.edges().size(); ++i)
    {
        const auto& e = topo.edges()[i];

        std::cout << "edge " << i << ": ["
                  << e[0] << ", "
                  << e[1] << "]\n";
    }

    std::cout << "\n--- faces ---\n";
    for (std::size_t i = 0; i < topo.faces().size(); ++i)
    {
        std::cout << "face " << i << ": ";
        print_vector(topo.faces()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- cell2edge ---\n";
    for (std::size_t i = 0; i < topo.cell2edge().size(); ++i)
    {
        std::cout << "cell " << i << ": ";
        print_vector(topo.cell2edge()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- cell2face ---\n";
    for (std::size_t i = 0; i < topo.cell2face().size(); ++i)
    {
        std::cout << "cell " << i << ": ";
        print_vector(topo.cell2face()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- edge2cell ---\n";
    for (std::size_t i = 0; i < topo.edge2cell().size(); ++i)
    {
        std::cout << "edge " << i << ": ";
        print_vector(topo.edge2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- face2cell ---\n";
    for (std::size_t i = 0; i < topo.face2cell().size(); ++i)
    {
        std::cout << "face " << i << ": ";
        print_vector(topo.face2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- node2cell ---\n";
    for (std::size_t i = 0; i < topo.node2cell().size(); ++i)
    {
        std::cout << "node " << i << ": ";
        print_vector(topo.node2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- node2edge ---\n";
    for (std::size_t i = 0; i < topo.node2edge().size(); ++i)
    {
        std::cout << "node " << i << ": ";
        print_vector(topo.node2edge()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- node2face ---\n";
    for (std::size_t i = 0; i < topo.node2face().size(); ++i)
    {
        std::cout << "node " << i << ": ";
        print_vector(topo.node2face()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- node2node ---\n";
    for (std::size_t i = 0; i < topo.node2node().size(); ++i)
    {
        std::cout << "node " << i << ": ";
        print_vector(topo.node2node()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- face2edge ---\n";
    for (std::size_t i = 0; i < topo.face2edge().size(); ++i)
    {
        std::cout << "face " << i << ": ";
        print_vector(topo.face2edge()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- edge2face ---\n";
    for (std::size_t i = 0; i < topo.edge2face().size(); ++i)
    {
        std::cout << "edge " << i << ": ";
        print_vector(topo.edge2face()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- cell2cell ---\n";
    for (std::size_t i = 0; i < topo.cell2cell().size(); ++i)
    {
        std::cout << "cell " << i << ": ";
        print_vector(topo.cell2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- boundary ---\n";

    std::cout << "boundary_edges = ";
    print_vector(topo.boundary_edges());
    std::cout << "\n";

    std::cout << "boundary_faces = ";
    print_vector(topo.boundary_faces());
    std::cout << "\n";

    std::cout << "boundary_nodes = ";
    print_vector(topo.boundary_nodes());
    std::cout << "\n";

    std::cout << "boundary_cells = ";
    print_vector(topo.boundary_cells());
    std::cout << "\n";
}

int main()
{
    OpenCAX::HexMesh mesh =
        OpenCAX::HexMesh::create_structured_box(
            0.0, 1.0,
            0.0, 1.0,
            0.0, 1.0,
            2, 2, 2
        );

    std::cout << "===== HEX MESH =====\n";

    std::cout << "nodes  = " << mesh.num_nodes() << "\n";
    std::cout << "cells  = " << mesh.num_cells() << "\n";
    std::cout << "volume = " << mesh.total_volume() << "\n";

    std::cout << "bounds = ";
    print_array6(mesh.bounds());
    std::cout << "\n";

    std::string error;
    if (!mesh.validate(&error))
    {
        std::cerr << "invalid hex mesh: " << error << "\n";
        return 1;
    }

    std::cout << "\n===== CELLS =====\n";
    for (std::size_t i = 0; i < mesh.num_cells(); ++i)
    {
        std::cout << "cell " << i << ":\n";

        std::cout << "  volume   = "
                  << mesh.volume(static_cast<int>(i)) << "\n";

        std::cout << "  centroid = ";
        print_array3(mesh.centroid(static_cast<int>(i)));
        std::cout << "\n";

        std::cout << "  nodes    = ";
        print_vector(mesh.cells()[i].node_ids);
        std::cout << "\n";
    }

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    std::cout << "\n===== SUMMARY =====\n";
    std::cout << "edges = " << topo.num_edges() << "\n";
    std::cout << "faces = " << topo.num_faces() << "\n";

    std::cout << "boundary edges = "
              << topo.boundary_edges().size() << "\n";

    std::cout << "boundary faces = "
              << topo.boundary_faces().size() << "\n";

    std::cout << "boundary nodes = "
              << topo.boundary_nodes().size() << "\n";

    std::cout << "boundary cells = "
              << topo.boundary_cells().size() << "\n";

    // 打印完整拓扑关系
    print_topology(topo);

    OpenCAX::VtkViewer::showMeshWithIds(mesh);

    return 0;
}
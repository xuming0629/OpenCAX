#include <OpenCAX/Mesh/QuadMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Post/VtkViewer.h>

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
 * 打印拓扑关系
 * ========================= */
static void print_topology(
    const OpenCAX::MeshTopology& topo
)
{
    std::cout << "\n===== TOPOLOGY =====\n";

    std::cout << "edges = " << topo.num_edges() << "\n";

    std::cout << "\n--- edges ---\n";
    for (std::size_t i = 0; i < topo.edges().size(); ++i)
    {
        const auto& e = topo.edges()[i];

        std::cout << i << ": ["
                  << e[0] << ", "
                  << e[1] << "]";

        // if (topo.boundary_edge(i))
        // {
        //     std::cout << "  boundary";
        // }

        std::cout << "\n";
    }

    std::cout << "\n--- cell2edge ---\n";
    for (std::size_t i = 0; i < topo.cell2edge().size(); ++i)
    {
        std::cout << "cell " << i << ": ";
        print_vector(topo.cell2edge()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- edge2cell ---\n";
    for (std::size_t i = 0; i < topo.edge2cell().size(); ++i)
    {
        std::cout << "edge " << i << ": ";
        print_vector(topo.edge2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- node2cell ---\n";
    for (std::size_t i = 0; i < topo.node2cell().size(); ++i)
    {
        std::cout << "node " << i << ": ";
        print_vector(topo.node2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- node2node ---\n";
    for (std::size_t i = 0; i < topo.node2node().size(); ++i)
    {
        std::cout << "node " << i << ": ";
        print_vector(topo.node2node()[i]);
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

    std::cout << "boundary_nodes = ";
    print_vector(topo.boundary_nodes());
    std::cout << "\n";

    std::cout << "boundary_cells = ";
    print_vector(topo.boundary_cells());
    std::cout << "\n";
}

int main()
{
    OpenCAX::QuadMesh mesh =
        OpenCAX::QuadMesh::create_structured_rectangle(
            0.0, 1.0,
            0.0, 1.0,
            4, 4
        );

    std::cout << "nodes = " << mesh.num_nodes() << "\n";
    std::cout << "cells = " << mesh.num_cells() << "\n";
    std::cout << "area  = " << mesh.total_area() << "\n";

    std::string error;
    if (!mesh.validate(&error))
    {
        std::cerr << "invalid quad mesh: " << error << "\n";
        return 1;
    }

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    std::cout << "edges = " << topo.num_edges() << "\n";
    std::cout << "boundary edges = "
              << topo.boundary_edges().size() << "\n";

    // 打印完整拓扑关系
    print_topology(topo);

    OpenCAX::VtkViewer::showMeshWithIds(mesh);

    return 0;
}
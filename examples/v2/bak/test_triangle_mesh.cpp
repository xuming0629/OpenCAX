#include <OpenCAX/Mesh/MeshModule.h>
#include <OpenCAX/Post/VtkViewer.h>
#include <array>
#include <iostream>
#include <vector>


#include <iomanip>

static void print_mesh_nodes(const OpenCAX::Mesh& mesh)
{
    std::cout << "\n===== nodes =====" << std::endl;

    for (const auto& node : mesh.nodes()) {
        std::cout << "node " << node.id << ": "
                  << "(" << node.x << ", "
                  << node.y << ", "
                  << node.z << ")"
                  << std::endl;
    }
}

static void print_mesh_cells(const OpenCAX::Mesh& mesh)
{
    std::cout << "\n===== cells =====" << std::endl;

    for (const auto& cell : mesh.cells()) {
        std::cout << "cell " << cell.id << ": ";

        for (int nid : cell.node_ids) {
            std::cout << nid << " ";
        }

        std::cout << std::endl;
    }
}

static void print_vector_matrix(
    const std::string& name,
    const std::vector<std::vector<int>>& mat
)
{
    std::cout << "\n===== " << name << " =====" << std::endl;

    for (std::size_t i = 0; i < mat.size(); ++i) {
        std::cout << name << "[" << i << "] = ";

        for (int v : mat[i]) {
            std::cout << v << " ";
        }

        std::cout << std::endl;
    }
}

static void print_edges(
    const OpenCAX::MeshTopology& topo
)
{
    std::cout << "\n===== edge2node =====" << std::endl;

    const auto& edges = topo.edges();

    for (std::size_t i = 0; i < edges.size(); ++i) {
        std::cout << "edge " << i << ": "
                  << edges[i][0] << " "
                  << edges[i][1] << std::endl;
    }
}

static void print_boundary(
    const OpenCAX::MeshTopology& topo
)
{
    std::cout << "\n===== boundary_edges =====" << std::endl;

    for (int eid : topo.boundary_edges()) {
        std::cout << eid << " ";
    }

    std::cout << std::endl;

    std::cout << "\n===== boundary_nodes =====" << std::endl;

    for (int nid : topo.boundary_nodes()) {
        std::cout << nid << " ";
    }

    std::cout << std::endl;
}
int main()
{
    using namespace OpenCAX;

    TriangleMesh structured =
        TriangleMesh::create_structured_rectangle(
            0.0, 1.0,
            0.0, 1.0,
            2, 2
        );

    MeshTopology structured_topology;
    structured_topology.build(structured);

    std::cout << "===== structured triangle mesh =====" << std::endl;
    std::cout << "nodes = " << structured.num_nodes() << std::endl;
    std::cout << "cells = " << structured.num_cells() << std::endl;
    std::cout << "triangles = " << structured.num_triangles() << std::endl;
    std::cout << "edges = " << structured.edges().size() << std::endl;
    std::cout << "topology edges = " << structured_topology.num_edges() << std::endl;
    std::cout << "boundary edges = " << structured.boundary_edges().size() << std::endl;
    std::cout << "topology boundary edges = " << structured_topology.boundary_edges().size() << std::endl;
    std::cout << "boundary nodes = " << structured.boundary_nodes().size() << std::endl;
    std::cout << "area = " << structured.total_area() << std::endl;

    print_mesh_nodes(structured);
    print_mesh_cells(structured);

    print_edges(structured_topology);
    

    print_vector_matrix(
        "cell2edge",
        structured_topology.cell2edge()
    );

    print_vector_matrix(
        "edge2cell",
        structured_topology.edge2cell()
    );

    print_vector_matrix(
        "node2cell",
        structured_topology.node2cell()
    );

    print_vector_matrix(
        "node2node",
        structured_topology.node2node()
    );

    print_boundary(structured_topology);

    VisualOptions options;
    options.show_node_ids = true;
    options.show_cell_ids = true;
    options.show_points = true;
    options.wireframe = true;
    options.point_size = 5.0;
    options.line_width = 1.5;

    VtkViewer::showMesh(structured, options);

    return 0;
}
/**
 * @file test_triangle_mesh_with_topology.cpp
 * @brief OpenCAX TriangleMesh + MeshTopology + VTK 可视化完整测试
 */

#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Post/VtkViewer.h>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace OpenCAX;

/* =========================
 * 打印工具
 * ========================= */
static void print_vector_int(const std::vector<int>& v)
{
    std::cout << "[";
    for (size_t i = 0; i < v.size(); ++i)
    {
        std::cout << v[i];
        if (i + 1 < v.size()) std::cout << ", ";
    }
    std::cout << "]";
}

/* =========================
 * 三角形有向面积
 * ========================= */
static double signed_area2_xy(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    return (b.x - a.x) * (c.y - a.y)
         - (b.y - a.y) * (c.x - a.x);
}

/* =========================
 * 检查 CCW + 直角点
 * ========================= */
static bool check_triangles(const TriangleMesh& mesh)
{
    const auto& nodes = mesh.nodes();
    const auto& cells = mesh.cells();

    std::cout << "\n===== Triangle Check =====\n";

    bool ok = true;

    for (const auto& cell : cells)
    {
        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];

        const auto& a = nodes[n0];
        const auto& b = nodes[n1];
        const auto& c = nodes[n2];

        double area2 = signed_area2_xy(a, b, c);
        bool ccw = area2 > 0;

        // b 是否直角点
        double bax = a.x - b.x;
        double bay = a.y - b.y;
        double bcx = c.x - b.x;
        double bcy = c.y - b.y;

        bool right = std::abs(bax * bcx + bay * bcy) < 1e-12;

        std::cout << "cell " << cell.id
                  << " nodes=";
        print_vector_int(cell.node_ids);

        std::cout << " area2=" << area2
                  << " ccw=" << ccw
                  << " right_at_middle=" << right
                  << "\n";

        if (!ccw || !right) ok = false;
    }

    return ok;
}

/* =========================
 * 打印 Mesh
 * ========================= */
static void print_mesh(const TriangleMesh& mesh)
{
    std::cout << "\n===== Nodes =====\n";
    for (const auto& n : mesh.nodes())
    {
        std::cout << "node " << n.id
                  << " (" << n.x << "," << n.y << ")\n";
    }

    std::cout << "\n===== Cells =====\n";
    for (const auto& c : mesh.cells())
    {
        std::cout << "cell " << c.id << " : ";
        print_vector_int(c.node_ids);
        std::cout << "\n";
    }
}

/* =========================
 * 打印拓扑
 * ========================= */
static void print_topology(
    const TriangleMesh& mesh,
    const MeshTopology& topo
)
{
    std::cout << "\n===== TOPOLOGY =====\n";

    std::cout << "edges = " << topo.num_edges() << "\n";

    std::cout << "\n--- edges ---\n";
    for (size_t i = 0; i < topo.edges().size(); ++i)
    {
        auto e = topo.edges()[i];
        std::cout << i << ": [" << e[0] << "," << e[1] << "]\n";
    }

    std::cout << "\n--- cell2edge ---\n";
    for (size_t i = 0; i < topo.cell2edge().size(); ++i)
    {
        std::cout << i << ": ";
        print_vector_int(topo.cell2edge()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- edge2cell ---\n";
    for (size_t i = 0; i < topo.edge2cell().size(); ++i)
    {
        std::cout << i << ": ";
        print_vector_int(topo.edge2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- node2cell ---\n";
    for (size_t i = 0; i < topo.node2cell().size(); ++i)
    {
        std::cout << i << ": ";
        print_vector_int(topo.node2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- node2node ---\n";
    for (size_t i = 0; i < topo.node2node().size(); ++i)
    {
        std::cout << i << ": ";
        print_vector_int(topo.node2node()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- cell2cell ---\n";
    for (size_t i = 0; i < topo.cell2cell().size(); ++i)
    {
        std::cout << i << ": ";
        print_vector_int(topo.cell2cell()[i]);
        std::cout << "\n";
    }

    std::cout << "\n--- boundary ---\n";
    std::cout << "boundary_edges = ";
    print_vector_int(topo.boundary_edges());
    std::cout << "\n";

    std::cout << "boundary_nodes = ";
    print_vector_int(topo.boundary_nodes());
    std::cout << "\n";

    std::cout << "boundary_cells = ";
    print_vector_int(topo.boundary_cells());
    std::cout << "\n";
}

/* =========================
 * main
 * ========================= */
int main()
{
    std::cout << "===== OpenCAX Full Mesh Test =====\n";

    TriangleMesh mesh = TriangleMesh::create_structured_rectangle(
        0.0, 1.0,
        0.0, 1.0,
        4, 4,
        false
    );

    std::cout << "nodes = " << mesh.num_nodes() << "\n";
    std::cout << "cells = " << mesh.num_cells() << "\n";

    print_mesh(mesh);

    /* =========================
     * 拓扑构建
     * ========================= */
    MeshTopology topo;
    topo.build(mesh);

    print_topology(mesh, topo);

    /* =========================
     * 几何检查
     * ========================= */
    if (!check_triangles(mesh))
    {
        std::cerr << "[FAILED] triangle check failed\n";
        return -1;
    }

    /* =========================
     * VTK 可视化
     * ========================= */
    VisualOptions opt;
    opt.show_node_ids = true;
    opt.show_cell_ids = true;
    opt.show_points = true;
    opt.wireframe = true;
    opt.point_size = 5.0;
    opt.line_width = 1.5;

    VtkViewer::showMesh(mesh, opt);

    std::cout << "\n[PASSED] all tests done\n";

    return 0;
}
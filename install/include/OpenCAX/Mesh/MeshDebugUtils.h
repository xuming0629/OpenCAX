#pragma once

/**
 * @file MeshDebugUtils.h
 * @brief OpenCAX 通用网格调试工具
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <array>
#include <iostream>
#include <iosfwd>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API MeshDebugUtils
{
public:
    static void print_vector_int(
        const std::vector<int>& v,
        std::ostream& os
    );

    static void print_vector_int(
        const std::vector<int>& v
    );

    template <typename MeshType>
    static void print_mesh(
        const MeshType& mesh,
        std::ostream& os
    );

    template <typename MeshType>
    static void print_mesh(
        const MeshType& mesh
    );

    template <typename MeshType>
    static bool check_mesh(
        const MeshType& mesh,
        std::ostream& os,
        double eps = 1.0e-12
    );

    template <typename MeshType>
    static bool check_mesh(
        const MeshType& mesh,
        double eps = 1.0e-12
    );

    static void print_topology(
        const MeshTopology& topo,
        std::ostream& os
    );

    static void print_topology(
        const MeshTopology& topo
    );

    template <typename MeshType>
    static void print_topology(
        const MeshType& mesh,
        const MeshTopology& topo,
        std::ostream& os
    );

    template <typename MeshType>
    static void print_topology(
        const MeshType& mesh,
        const MeshTopology& topo
    );

    static double signed_area2_xy(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

    static double triangle_area(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

    static double signed_tetra_volume6(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c,
        const MeshNode& d
    );

    static double tetra_volume(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c,
        const MeshNode& d
    );

private:
    MeshDebugUtils() = delete;

    static const char* cell_type_name(
        CellType type
    );

    static bool check_triangle_cell(
        const Mesh& mesh,
        const MeshCell& cell,
        std::ostream& os,
        double eps
    );

    static bool check_quad_cell(
        const Mesh& mesh,
        const MeshCell& cell,
        std::ostream& os,
        double eps
    );

    static bool check_tetra_cell(
        const Mesh& mesh,
        const MeshCell& cell,
        std::ostream& os,
        double eps
    );

    static bool check_hexa_cell(
        const Mesh& mesh,
        const MeshCell& cell,
        std::ostream& os,
        double eps
    );

    static bool check_node_ids(
        const Mesh& mesh,
        const MeshCell& cell,
        int expected_node_count,
        std::ostream& os
    );

    static std::array<double, 3> sub(
        const MeshNode& a,
        const MeshNode& b
    );

    static std::array<double, 3> cross(
        const std::array<double, 3>& a,
        const std::array<double, 3>& b
    );

    static double dot(
        const std::array<double, 3>& a,
        const std::array<double, 3>& b
    );
};

/* ============================================================
 * 模板函数必须放在头文件中
 * ============================================================ */

template <typename MeshType>
void MeshDebugUtils::print_mesh(
    const MeshType& mesh,
    std::ostream& os
)
{
    os << "\n===== Mesh Info =====\n";
    os << "nodes = " << mesh.num_nodes() << "\n";
    os << "cells = " << mesh.num_cells() << "\n";

    os << "\n===== Nodes =====\n";

    for (const auto& n : mesh.nodes())
    {
        os << "node " << n.id
           << " (" << n.x
           << ", " << n.y
           << ", " << n.z
           << ")\n";
    }

    os << "\n===== Cells =====\n";

    for (const auto& c : mesh.cells())
    {
        os << "cell " << c.id
           << " type=" << cell_type_name(c.type)
           << " nodes=";

        print_vector_int(c.node_ids, os);

        os << "\n";
    }
}

template <typename MeshType>
void MeshDebugUtils::print_mesh(
    const MeshType& mesh
)
{
    print_mesh(mesh, std::cout);
}

template <typename MeshType>
bool MeshDebugUtils::check_mesh(
    const MeshType& mesh,
    std::ostream& os,
    double eps
)
{
    os << "\n===== Mesh Check =====\n";

    bool ok = true;

    for (const auto& cell : mesh.cells())
    {
        bool cell_ok = false;

        switch (cell.type)
        {
            case CellType::Triangle3:
            {
                cell_ok = check_triangle_cell(
                    mesh,
                    cell,
                    os,
                    eps
                );
                break;
            }

            case CellType::Quad4:
            {
                cell_ok = check_quad_cell(
                    mesh,
                    cell,
                    os,
                    eps
                );
                break;
            }

            case CellType::Tetra4:
            {
                cell_ok = check_tetra_cell(
                    mesh,
                    cell,
                    os,
                    eps
                );
                break;
            }

            case CellType::Hexa8:
            {
                cell_ok = check_hexa_cell(
                    mesh,
                    cell,
                    os,
                    eps
                );
                break;
            }

            default:
            {
                os << "cell " << cell.id
                   << " unsupported cell type="
                   << cell_type_name(cell.type)
                   << " nodes=";

                print_vector_int(cell.node_ids, os);
                os << "\n";

                cell_ok = false;
                break;
            }
        }

        if (!cell_ok)
        {
            ok = false;
        }
    }

    os << "\nmesh check ok = "
       << (ok ? "true" : "false")
       << "\n";

    return ok;
}

template <typename MeshType>
bool MeshDebugUtils::check_mesh(
    const MeshType& mesh,
    double eps
)
{
    return check_mesh(
        mesh,
        std::cout,
        eps
    );
}

template <typename MeshType>
void MeshDebugUtils::print_topology(
    const MeshType& mesh,
    const MeshTopology& topo,
    std::ostream& os
)
{
    (void)mesh;
    print_topology(topo, os);
}

template <typename MeshType>
void MeshDebugUtils::print_topology(
    const MeshType& mesh,
    const MeshTopology& topo
)
{
    (void)mesh;
    print_topology(topo, std::cout);
}

} // namespace OpenCAX
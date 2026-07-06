/**
 * @file MeshDebugUtils.cpp
 * @brief OpenCAX 通用网格调试工具实现
 */

#include <OpenCAX/Mesh/MeshDebugUtils.h>

#include <cmath>
#include <iostream>
#include <ostream>
#include <set>

namespace OpenCAX
{

void MeshDebugUtils::print_vector_int(
    const std::vector<int>& v,
    std::ostream& os
)
{
    os << "[";

    for (std::size_t i = 0; i < v.size(); ++i)
    {
        os << v[i];

        if (i + 1 < v.size())
        {
            os << ", ";
        }
    }

    os << "]";
}

void MeshDebugUtils::print_vector_int(
    const std::vector<int>& v
)
{
    print_vector_int(v, std::cout);
}

const char* MeshDebugUtils::cell_type_name(
    CellType type
)
{
    switch (type)
    {
        case CellType::Vertex1:
            return "Vertex1";

        case CellType::Line2:
            return "Line2";

        case CellType::Triangle3:
            return "Triangle3";

        case CellType::Quad4:
            return "Quad4";

        case CellType::Tetra4:
            return "Tetra4";

        case CellType::Hexa8:
            return "Hexa8";

        default:
            return "Unknown";
    }
}

std::array<double, 3> MeshDebugUtils::sub(
    const MeshNode& a,
    const MeshNode& b
)
{
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

std::array<double, 3> MeshDebugUtils::cross(
    const std::array<double, 3>& a,
    const std::array<double, 3>& b
)
{
    return {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    };
}

double MeshDebugUtils::dot(
    const std::array<double, 3>& a,
    const std::array<double, 3>& b
)
{
    return a[0] * b[0] +
           a[1] * b[1] +
           a[2] * b[2];
}

double MeshDebugUtils::signed_area2_xy(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    return (b.x - a.x) * (c.y - a.y)
         - (b.y - a.y) * (c.x - a.x);
}

double MeshDebugUtils::triangle_area(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    const auto ab = sub(b, a);
    const auto ac = sub(c, a);

    const auto cr = cross(ab, ac);

    return 0.5 * std::sqrt(dot(cr, cr));
}

double MeshDebugUtils::signed_tetra_volume6(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c,
    const MeshNode& d
)
{
    const auto ab = sub(b, a);
    const auto ac = sub(c, a);
    const auto ad = sub(d, a);

    return dot(ab, cross(ac, ad));
}

double MeshDebugUtils::tetra_volume(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c,
    const MeshNode& d
)
{
    return std::abs(
        signed_tetra_volume6(a, b, c, d)
    ) / 6.0;
}

bool MeshDebugUtils::check_node_ids(
    const Mesh& mesh,
    const MeshCell& cell,
    int expected_node_count,
    std::ostream& os
)
{
    if (static_cast<int>(cell.node_ids.size()) != expected_node_count)
    {
        os << "cell " << cell.id
           << " invalid node count. expected="
           << expected_node_count
           << " actual="
           << cell.node_ids.size()
           << " nodes=";

        print_vector_int(cell.node_ids, os);
        os << "\n";

        return false;
    }

    std::set<int> unique_ids;

    for (int node_id : cell.node_ids)
    {
        if (!mesh.valid_node_id(node_id))
        {
            os << "cell " << cell.id
               << " invalid node id="
               << node_id
               << " nodes=";

            print_vector_int(cell.node_ids, os);
            os << "\n";

            return false;
        }

        unique_ids.insert(node_id);
    }

    if (static_cast<int>(unique_ids.size()) != expected_node_count)
    {
        os << "cell " << cell.id
           << " duplicated node ids: ";

        print_vector_int(cell.node_ids, os);
        os << "\n";

        return false;
    }

    return true;
}

bool MeshDebugUtils::check_triangle_cell(
    const Mesh& mesh,
    const MeshCell& cell,
    std::ostream& os,
    double eps
)
{
    if (!check_node_ids(mesh, cell, 3, os))
    {
        return false;
    }

    const auto& nodes = mesh.nodes();

    const int n0 = cell.node_ids[0];
    const int n1 = cell.node_ids[1];
    const int n2 = cell.node_ids[2];

    const auto& a = nodes[static_cast<std::size_t>(n0)];
    const auto& b = nodes[static_cast<std::size_t>(n1)];
    const auto& c = nodes[static_cast<std::size_t>(n2)];

    const double area = triangle_area(a, b, c);
    const double area2_xy = signed_area2_xy(a, b, c);

    const bool non_degenerate = area > eps;
    const bool ccw_xy = area2_xy > eps;

    os << "cell " << cell.id
       << " type=Triangle3 nodes=";

    print_vector_int(cell.node_ids, os);

    os << " area=" << area
       << " area2_xy=" << area2_xy
       << " ccw_xy=" << (ccw_xy ? "true" : "false")
       << " non_degenerate=" << (non_degenerate ? "true" : "false")
       << "\n";

    return non_degenerate && ccw_xy;
}

bool MeshDebugUtils::check_quad_cell(
    const Mesh& mesh,
    const MeshCell& cell,
    std::ostream& os,
    double eps
)
{
    if (!check_node_ids(mesh, cell, 4, os))
    {
        return false;
    }

    const auto& nodes = mesh.nodes();

    const int n0 = cell.node_ids[0];
    const int n1 = cell.node_ids[1];
    const int n2 = cell.node_ids[2];
    const int n3 = cell.node_ids[3];

    const auto& p0 = nodes[static_cast<std::size_t>(n0)];
    const auto& p1 = nodes[static_cast<std::size_t>(n1)];
    const auto& p2 = nodes[static_cast<std::size_t>(n2)];
    const auto& p3 = nodes[static_cast<std::size_t>(n3)];

    const double area =
        triangle_area(p0, p1, p2) +
        triangle_area(p0, p2, p3);

    const double s0 = signed_area2_xy(p0, p1, p2);
    const double s1 = signed_area2_xy(p1, p2, p3);
    const double s2 = signed_area2_xy(p2, p3, p0);
    const double s3 = signed_area2_xy(p3, p0, p1);

    const bool non_degenerate = area > eps;

    const bool all_positive =
        s0 > eps &&
        s1 > eps &&
        s2 > eps &&
        s3 > eps;

    const bool all_negative =
        s0 < -eps &&
        s1 < -eps &&
        s2 < -eps &&
        s3 < -eps;

    const bool convex =
        all_positive || all_negative;

    const bool ccw_xy =
        all_positive;

    os << "cell " << cell.id
       << " type=Quad4 nodes=";

    print_vector_int(cell.node_ids, os);

    os << " area=" << area
       << " convex=" << (convex ? "true" : "false")
       << " ccw_xy=" << (ccw_xy ? "true" : "false")
       << " signs=["
       << s0 << ", "
       << s1 << ", "
       << s2 << ", "
       << s3 << "]"
       << " non_degenerate=" << (non_degenerate ? "true" : "false")
       << "\n";

    return non_degenerate && convex && ccw_xy;
}

bool MeshDebugUtils::check_tetra_cell(
    const Mesh& mesh,
    const MeshCell& cell,
    std::ostream& os,
    double eps
)
{
    if (!check_node_ids(mesh, cell, 4, os))
    {
        return false;
    }

    const auto& nodes = mesh.nodes();

    const int n0 = cell.node_ids[0];
    const int n1 = cell.node_ids[1];
    const int n2 = cell.node_ids[2];
    const int n3 = cell.node_ids[3];

    const auto& a = nodes[static_cast<std::size_t>(n0)];
    const auto& b = nodes[static_cast<std::size_t>(n1)];
    const auto& c = nodes[static_cast<std::size_t>(n2)];
    const auto& d = nodes[static_cast<std::size_t>(n3)];

    const double volume6 =
        signed_tetra_volume6(a, b, c, d);

    const double volume =
        std::abs(volume6) / 6.0;

    const bool non_degenerate =
        volume > eps;

    const bool positive_orientation =
        volume6 > eps;

    os << "cell " << cell.id
       << " type=Tetra4 nodes=";

    print_vector_int(cell.node_ids, os);

    os << " volume=" << volume
       << " volume6=" << volume6
       << " positive_orientation="
       << (positive_orientation ? "true" : "false")
       << " non_degenerate="
       << (non_degenerate ? "true" : "false")
       << "\n";

    return non_degenerate && positive_orientation;
}

bool MeshDebugUtils::check_hexa_cell(
    const Mesh& mesh,
    const MeshCell& cell,
    std::ostream& os,
    double eps
)
{
    if (!check_node_ids(mesh, cell, 8, os))
    {
        return false;
    }

    const auto& nodes = mesh.nodes();

    const int n0 = cell.node_ids[0];
    const int n1 = cell.node_ids[1];
    const int n2 = cell.node_ids[2];
    const int n3 = cell.node_ids[3];
    const int n4 = cell.node_ids[4];
    const int n5 = cell.node_ids[5];
    const int n6 = cell.node_ids[6];
    const int n7 = cell.node_ids[7];

    const auto& p0 = nodes[static_cast<std::size_t>(n0)];
    const auto& p1 = nodes[static_cast<std::size_t>(n1)];
    const auto& p2 = nodes[static_cast<std::size_t>(n2)];
    const auto& p3 = nodes[static_cast<std::size_t>(n3)];
    const auto& p4 = nodes[static_cast<std::size_t>(n4)];
    const auto& p5 = nodes[static_cast<std::size_t>(n5)];
    const auto& p6 = nodes[static_cast<std::size_t>(n6)];
    const auto& p7 = nodes[static_cast<std::size_t>(n7)];

    const double v0 = tetra_volume(p0, p1, p3, p4);
    const double v1 = tetra_volume(p1, p2, p3, p6);
    const double v2 = tetra_volume(p1, p3, p4, p6);
    const double v3 = tetra_volume(p1, p4, p5, p6);
    const double v4 = tetra_volume(p3, p4, p6, p7);

    const double volume =
        v0 + v1 + v2 + v3 + v4;

    const auto e1 = sub(p1, p0);
    const auto e2 = sub(p3, p0);
    const auto e3 = sub(p4, p0);

    const double jac =
        dot(e1, cross(e2, e3));

    const bool non_degenerate =
        volume > eps;

    const bool positive_jacobian =
        jac > eps;

    os << "cell " << cell.id
       << " type=Hexa8 nodes=";

    print_vector_int(cell.node_ids, os);

    os << " volume=" << volume
       << " jacobian_at_p0=" << jac
       << " positive_jacobian="
       << (positive_jacobian ? "true" : "false")
       << " non_degenerate="
       << (non_degenerate ? "true" : "false")
       << "\n";

    return non_degenerate && positive_jacobian;
}

void MeshDebugUtils::print_topology(
    const MeshTopology& topo,
    std::ostream& os
)
{
    os << "\n===== TOPOLOGY =====\n";

    os << "edges = "
       << topo.num_edges()
       << "\n";

    os << "\n--- edges ---\n";

    for (std::size_t i = 0; i < topo.edges().size(); ++i)
    {
        const auto e = topo.edges()[i];

        os << i
           << ": ["
           << e[0]
           << ", "
           << e[1]
           << "]\n";
    }

    os << "\n--- cell2edge ---\n";

    for (std::size_t i = 0; i < topo.cell2edge().size(); ++i)
    {
        os << i << ": ";
        print_vector_int(topo.cell2edge()[i], os);
        os << "\n";
    }

    os << "\n--- edge2cell ---\n";

    for (std::size_t i = 0; i < topo.edge2cell().size(); ++i)
    {
        os << i << ": ";
        print_vector_int(topo.edge2cell()[i], os);
        os << "\n";
    }

    os << "\n--- node2cell ---\n";

    for (std::size_t i = 0; i < topo.node2cell().size(); ++i)
    {
        os << i << ": ";
        print_vector_int(topo.node2cell()[i], os);
        os << "\n";
    }

    os << "\n--- node2node ---\n";

    for (std::size_t i = 0; i < topo.node2node().size(); ++i)
    {
        os << i << ": ";
        print_vector_int(topo.node2node()[i], os);
        os << "\n";
    }

    os << "\n--- cell2cell ---\n";

    for (std::size_t i = 0; i < topo.cell2cell().size(); ++i)
    {
        os << i << ": ";
        print_vector_int(topo.cell2cell()[i], os);
        os << "\n";
    }

    os << "\n--- boundary ---\n";

    os << "boundary_edges = ";
    print_vector_int(topo.boundary_edges(), os);
    os << "\n";

    os << "boundary_nodes = ";
    print_vector_int(topo.boundary_nodes(), os);
    os << "\n";

    os << "boundary_cells = ";
    print_vector_int(topo.boundary_cells(), os);
    os << "\n";
}

void MeshDebugUtils::print_topology(
    const MeshTopology& topo
)
{
    print_topology(topo, std::cout);
}

} // namespace OpenCAX
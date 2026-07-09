#include <OpenCAX/Mesh2/TriangleMesh.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace OpenCAX
{

namespace
{
static double signed_area2_xy(const MeshNode& a, const MeshNode& b, const MeshNode& c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static std::array<int, 3> make_ccw_xy(const TriangleMesh& mesh, int a, int b, int c)
{
    const auto& nodes = mesh.nodes();
    if (signed_area2_xy(nodes[a], nodes[b], nodes[c]) < 0.0) return {a, c, b};
    return {a, b, c};
}
}

TriangleMesh::TriangleMesh()
{
    info_.name = "TriangleMesh";
    info_.source = "OpenCAX";
    info_.dimension = MeshDimension::Dim2;
}

TriangleMesh TriangleMesh::create_structured_rectangle(double xmin, double xmax, double ymin, double ymax, int nx, int ny, bool alternate_diagonal)
{
    if (nx <= 0 || ny <= 0) throw std::invalid_argument("TriangleMesh::create_structured_rectangle: nx/ny must be positive.");
    if (!(xmax > xmin) || !(ymax > ymin)) throw std::invalid_argument("TriangleMesh::create_structured_rectangle: invalid bounds.");

    TriangleMesh mesh;
    mesh.info_.name = "StructuredTriangleMesh";
    mesh.info_.source = "OpenCAX::Structured";
    mesh.info_.source_type = MeshSourceType::Structured;
    mesh.info_.structured = true;

    mesh.structured_info_ = {nx, ny, xmin, xmax, ymin, ymax, alternate_diagonal};

    const double dx = (xmax - xmin) / static_cast<double>(nx);
    const double dy = (ymax - ymin) / static_cast<double>(ny);

    auto nid = [nx](int i, int j) { return j * (nx + 1) + i; };

    for (int j = 0; j <= ny; ++j)
    {
        for (int i = 0; i <= nx; ++i)
        {
            mesh.add_node(xmin + i * dx, ymin + j * dy, 0.0);
        }
    }

    for (int j = 0; j < ny; ++j)
    {
        for (int i = 0; i < nx; ++i)
        {
            const int n00 = nid(i, j);
            const int n10 = nid(i + 1, j);
            const int n01 = nid(i, j + 1);
            const int n11 = nid(i + 1, j + 1);

            const bool flip = alternate_diagonal && ((i + j) % 2 == 1);
            if (!flip)
            {
                mesh.add_cell(CellType::Triangle3, {n00, n10, n11});
                mesh.add_cell(CellType::Triangle3, {n11, n01, n00});
            }
            else
            {
                mesh.add_cell(CellType::Triangle3, {n01, n00, n10});
                mesh.add_cell(CellType::Triangle3, {n10, n11, n01});
            }
        }
    }
    return mesh;
}

TriangleMesh TriangleMesh::create_unstructured(const std::vector<std::array<double, 3>>& points, const std::vector<std::array<int, 3>>& triangles)
{
    if (points.empty()) throw std::invalid_argument("TriangleMesh::create_unstructured: points is empty.");
    if (triangles.empty()) throw std::invalid_argument("TriangleMesh::create_unstructured: triangles is empty.");

    TriangleMesh mesh;
    mesh.info_.name = "UnstructuredTriangleMesh";
    mesh.info_.source = "OpenCAX::Unstructured";
    mesh.info_.source_type = MeshSourceType::Unstructured;

    for (const auto& p : points) mesh.add_node(p[0], p[1], p[2]);

    for (const auto& t : triangles)
    {
        if (!mesh.valid_node_id(t[0]) || !mesh.valid_node_id(t[1]) || !mesh.valid_node_id(t[2])) throw std::out_of_range("TriangleMesh::create_unstructured: invalid node id.");
        if (t[0] == t[1] || t[1] == t[2] || t[2] == t[0]) throw std::invalid_argument("TriangleMesh::create_unstructured: duplicated node id.");
        const auto ccw = make_ccw_xy(mesh, t[0], t[1], t[2]);
        mesh.add_cell(CellType::Triangle3, {ccw[0], ccw[1], ccw[2]});
    }
    return mesh;
}

void TriangleMesh::set_structured_info(const StructuredGridInfo2D& info) { structured_info_ = info; }
const StructuredGridInfo2D& TriangleMesh::structured_info() const { return structured_info_; }
bool TriangleMesh::is_structured() const { return info_.source_type == MeshSourceType::Structured || info_.structured; }
bool TriangleMesh::is_unstructured() const { return info_.source_type == MeshSourceType::Unstructured || info_.source_type == MeshSourceType::Gmsh || info_.source_type == MeshSourceType::Imported; }

double TriangleMesh::triangle_area(const MeshNode& a, const MeshNode& b, const MeshNode& c)
{
    const double ux = b.x - a.x, uy = b.y - a.y, uz = b.z - a.z;
    const double vx = c.x - a.x, vy = c.y - a.y, vz = c.z - a.z;
    const double cx = uy * vz - uz * vy;
    const double cy = uz * vx - ux * vz;
    const double cz = ux * vy - uy * vx;
    return 0.5 * std::sqrt(cx * cx + cy * cy + cz * cz);
}

double TriangleMesh::area(int cell_id) const
{
    if (!valid_cell_id(cell_id)) return 0.0;
    const auto& c = cells_[cell_id];
    if ((c.type != CellType::Triangle3 && c.type != CellType::Triangle6) || c.node_ids.size() < 3) return 0.0;
    return triangle_area(nodes_[c.node_ids[0]], nodes_[c.node_ids[1]], nodes_[c.node_ids[2]]);
}

double TriangleMesh::total_area() const
{
    double s = 0.0;
    for (std::size_t i = 0; i < cells_.size(); ++i) s += area(static_cast<int>(i));
    return s;
}

std::array<double, 3> TriangleMesh::centroid(int cell_id) const
{
    if (!valid_cell_id(cell_id)) return {0,0,0};
    const auto& c = cells_[cell_id];
    if (c.node_ids.size() < 3) return {0,0,0};
    const auto& a = nodes_[c.node_ids[0]];
    const auto& b = nodes_[c.node_ids[1]];
    const auto& d = nodes_[c.node_ids[2]];
    return {(a.x + b.x + d.x) / 3.0, (a.y + b.y + d.y) / 3.0, (a.z + b.z + d.z) / 3.0};
}

bool TriangleMesh::validate(std::string* error_message, double area_eps) const
{
    if (!Mesh::validate(error_message)) return false;
    for (std::size_t i = 0; i < cells_.size(); ++i)
    {
        const auto& c = cells_[i];
        if (c.type != CellType::Triangle3 && c.type != CellType::Triangle6)
        {
            if (error_message) *error_message = "TriangleMesh::validate: unsupported triangle cell type.";
            return false;
        }
        if (area(static_cast<int>(i)) <= area_eps)
        {
            if (error_message) *error_message = "TriangleMesh::validate: non-positive triangle area.";
            return false;
        }
    }
    if (error_message) error_message->clear();
    return true;
}

bool TriangleMesh::validate(std::string* error_message) const
{
    return validate(error_message, 1.0e-14);
}

} // namespace OpenCAX

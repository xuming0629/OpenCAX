#include <OpenCAX/Mesh/QuadMesh.h>

#include <cmath>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace OpenCAX
{

QuadMesh::QuadMesh()
{
    source_type_ = QuadMeshSourceType::Unknown;
}

// =============================
// structured
// =============================
QuadMesh QuadMesh::create_structured_rectangle(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    int nx,
    int ny
)
{
    QuadMesh mesh;

    mesh.source_type_ = QuadMeshSourceType::Structured;

    mesh.structured_info_.nx = nx;
    mesh.structured_info_.ny = ny;
    mesh.structured_info_.xmin = xmin;
    mesh.structured_info_.xmax = xmax;
    mesh.structured_info_.ymin = ymin;
    mesh.structured_info_.ymax = ymax;

    const double dx = (xmax - xmin) / nx;
    const double dy = (ymax - ymin) / ny;

    // nodes
    for (int j = 0; j <= ny; ++j)
    {
        for (int i = 0; i <= nx; ++i)
        {
            mesh.add_node(
                xmin + i * dx,
                ymin + j * dy,
                0.0
            );
        }
    }

    auto nid = [nx](int i, int j)
    {
        return j * (nx + 1) + i;
    };

    // cells
    for (int j = 0; j < ny; ++j)
    {
        for (int i = 0; i < nx; ++i)
        {
            int n0 = nid(i, j);
            int n1 = nid(i + 1, j);
            int n2 = nid(i + 1, j + 1);
            int n3 = nid(i, j + 1);

            mesh.add_cell(CellType::Quad4, {n0, n1, n2, n3});
        }
    }

    return mesh;
}

// =============================
// unstructured
// =============================
QuadMesh QuadMesh::create_unstructured(
    const std::vector<std::array<double, 3>>& points,
    const std::vector<std::array<int, 4>>& quads
)
{
    QuadMesh mesh;

    mesh.source_type_ = QuadMeshSourceType::Unstructured;

    for (auto& p : points)
    {
        mesh.add_node(p[0], p[1], p[2]);
    }

    int n = (int)points.size();

    for (size_t i = 0; i < quads.size(); ++i)
    {
        const auto& q = quads[i];

        for (int k = 0; k < 4; ++k)
        {
            if (q[k] < 0 || q[k] >= n)
                throw std::runtime_error("invalid quad node index");
        }

        mesh.add_cell(CellType::Quad4, {q[0], q[1], q[2], q[3]});
    }

    return mesh;
}

// =============================
QuadMeshSourceType QuadMesh::source_type() const
{
    return source_type_;
}

void QuadMesh::set_source_type(QuadMeshSourceType type)
{
    source_type_ = type;
}

bool QuadMesh::is_structured() const
{
    return source_type_ == QuadMeshSourceType::Structured;
}

bool QuadMesh::is_unstructured() const
{
    return source_type_ == QuadMeshSourceType::Unstructured;
}

const QuadMeshStructuredInfo& QuadMesh::structured_info() const
{
    return structured_info_;
}

// =============================
// area
// =============================
double QuadMesh::triangle_area(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    double ux = b.x - a.x;
    double uy = b.y - a.y;
    double uz = b.z - a.z;

    double vx = c.x - a.x;
    double vy = c.y - a.y;
    double vz = c.z - a.z;

    double cx = uy * vz - uz * vy;
    double cy = uz * vx - ux * vz;
    double cz = ux * vy - uy * vx;

    return 0.5 * std::sqrt(cx * cx + cy * cy + cz * cz);
}

double QuadMesh::quad_area(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c,
    const MeshNode& d
)
{
    return triangle_area(a, b, c)
         + triangle_area(a, c, d);
}

double QuadMesh::area(int cell_id) const
{
    const auto& c = cells()[cell_id];

    const auto& n0 = nodes()[c.node_ids[0]];
    const auto& n1 = nodes()[c.node_ids[1]];
    const auto& n2 = nodes()[c.node_ids[2]];
    const auto& n3 = nodes()[c.node_ids[3]];

    return quad_area(n0, n1, n2, n3);
}

// =============================
double QuadMesh::total_area() const
{
    double s = 0.0;

    for (size_t i = 0; i < num_cells(); ++i)
        s += area(i);

    return s;
}

// =============================
std::array<double, 3> QuadMesh::centroid(int cell_id) const
{
    const auto& c = cells()[cell_id];

    const auto& p0 = nodes()[c.node_ids[0]];
    const auto& p1 = nodes()[c.node_ids[1]];
    const auto& p2 = nodes()[c.node_ids[2]];
    const auto& p3 = nodes()[c.node_ids[3]];

    return {
        0.25 * (p0.x + p1.x + p2.x + p3.x),
        0.25 * (p0.y + p1.y + p2.y + p3.y),
        0.25 * (p0.z + p1.z + p2.z + p3.z)
    };
}

// =============================
std::array<double, 6> QuadMesh::bounds() const
{
    double xmin = 1e30, ymin = 1e30, zmin = 1e30;
    double xmax = -1e30, ymax = -1e30, zmax = -1e30;

    for (size_t i = 0; i < num_nodes(); ++i)
    {
        const auto& p = nodes()[i];

        xmin = std::min(xmin, p.x);
        ymin = std::min(ymin, p.y);
        zmin = std::min(zmin, p.z);

        xmax = std::max(xmax, p.x);
        ymax = std::max(ymax, p.y);
        zmax = std::max(zmax, p.z);
    }

    return {xmin, xmax, ymin, ymax, zmin, zmax};
}

// =============================
bool QuadMesh::validate(
    std::string* error_message,
    double area_eps
) const
{
    for (size_t i = 0; i < num_cells(); ++i)
    {
        const auto& c = cells()[i];

        if (c.type != CellType::Quad4)
        {
            if (error_message)
                *error_message = "not quad4";
            return false;
        }

        double a = area(i);

        if (a < area_eps)
        {
            if (error_message)
                *error_message = "zero area quad";
            return false;
        }
    }

    return true;
}

} // namespace OpenCAX
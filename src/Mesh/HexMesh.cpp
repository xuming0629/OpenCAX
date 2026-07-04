#include <OpenCAX/Mesh/HexMesh.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace OpenCAX
{

HexMesh::HexMesh()
{
    info_.dimension = MeshDimension::Dim3;
    info_.source = "HexMesh";

    source_type_ = HexMeshSourceType::Unknown;
}

// =============================
// structured
// =============================
HexMesh HexMesh::create_structured_box(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    double zmin,
    double zmax,
    int nx,
    int ny,
    int nz
)
{
    if (nx <= 0)
    {
        throw std::invalid_argument(
            "HexMesh::create_structured_box: nx must be positive."
        );
    }

    if (ny <= 0)
    {
        throw std::invalid_argument(
            "HexMesh::create_structured_box: ny must be positive."
        );
    }

    if (nz <= 0)
    {
        throw std::invalid_argument(
            "HexMesh::create_structured_box: nz must be positive."
        );
    }

    if (!(xmax > xmin))
    {
        throw std::invalid_argument(
            "HexMesh::create_structured_box: xmax must be greater than xmin."
        );
    }

    if (!(ymax > ymin))
    {
        throw std::invalid_argument(
            "HexMesh::create_structured_box: ymax must be greater than ymin."
        );
    }

    if (!(zmax > zmin))
    {
        throw std::invalid_argument(
            "HexMesh::create_structured_box: zmax must be greater than zmin."
        );
    }

    HexMesh mesh;

    mesh.info_.name = "StructuredHexMesh";
    mesh.info_.source = "Structured";
    mesh.info_.dimension = MeshDimension::Dim3;

    mesh.source_type_ = HexMeshSourceType::Structured;

    mesh.structured_info_.nx = nx;
    mesh.structured_info_.ny = ny;
    mesh.structured_info_.nz = nz;

    mesh.structured_info_.xmin = xmin;
    mesh.structured_info_.xmax = xmax;

    mesh.structured_info_.ymin = ymin;
    mesh.structured_info_.ymax = ymax;

    mesh.structured_info_.zmin = zmin;
    mesh.structured_info_.zmax = zmax;

    const double dx = (xmax - xmin) / static_cast<double>(nx);
    const double dy = (ymax - ymin) / static_cast<double>(ny);
    const double dz = (zmax - zmin) / static_cast<double>(nz);

    // nodes
    for (int k = 0; k <= nz; ++k)
    {
        for (int j = 0; j <= ny; ++j)
        {
            for (int i = 0; i <= nx; ++i)
            {
                mesh.add_node(
                    xmin + static_cast<double>(i) * dx,
                    ymin + static_cast<double>(j) * dy,
                    zmin + static_cast<double>(k) * dz
                );
            }
        }
    }

    auto nid = [nx, ny](int i, int j, int k)
    {
        return k * (ny + 1) * (nx + 1)
             + j * (nx + 1)
             + i;
    };

    // cells
    for (int k = 0; k < nz; ++k)
    {
        for (int j = 0; j < ny; ++j)
        {
            for (int i = 0; i < nx; ++i)
            {
                int n0 = nid(i,     j,     k);
                int n1 = nid(i + 1, j,     k);
                int n2 = nid(i + 1, j + 1, k);
                int n3 = nid(i,     j + 1, k);

                int n4 = nid(i,     j,     k + 1);
                int n5 = nid(i + 1, j,     k + 1);
                int n6 = nid(i + 1, j + 1, k + 1);
                int n7 = nid(i,     j + 1, k + 1);

                /*
                 * Hexa8 节点顺序：
                 *
                 * bottom:
                 *
                 * n3 ---- n2
                 * |       |
                 * n0 ---- n1
                 *
                 * top:
                 *
                 * n7 ---- n6
                 * |       |
                 * n4 ---- n5
                 */
                mesh.add_cell(
                    CellType::Hexa8,
                    {n0, n1, n2, n3, n4, n5, n6, n7}
                );
            }
        }
    }

    return mesh;
}

// =============================
// unstructured
// =============================
HexMesh HexMesh::create_unstructured(
    const std::vector<std::array<double, 3>>& points,
    const std::vector<std::array<int, 8>>& hexes
)
{
    HexMesh mesh;

    mesh.info_.name = "UnstructuredHexMesh";
    mesh.info_.source = "Unstructured";
    mesh.info_.dimension = MeshDimension::Dim3;

    mesh.source_type_ = HexMeshSourceType::Unstructured;

    for (const auto& p : points)
    {
        mesh.add_node(p[0], p[1], p[2]);
    }

    int n = static_cast<int>(points.size());

    for (size_t i = 0; i < hexes.size(); ++i)
    {
        const auto& h = hexes[i];

        for (int k = 0; k < 8; ++k)
        {
            if (h[k] < 0 || h[k] >= n)
            {
                throw std::runtime_error(
                    "HexMesh::create_unstructured: invalid hexa node index."
                );
            }
        }

        mesh.add_cell(
            CellType::Hexa8,
            {h[0], h[1], h[2], h[3], h[4], h[5], h[6], h[7]}
        );
    }

    return mesh;
}

// =============================
HexMeshSourceType HexMesh::source_type() const
{
    return source_type_;
}

void HexMesh::set_source_type(
    HexMeshSourceType type
)
{
    source_type_ = type;
}

bool HexMesh::is_structured() const
{
    return source_type_ == HexMeshSourceType::Structured;
}

bool HexMesh::is_unstructured() const
{
    return source_type_ == HexMeshSourceType::Unstructured;
}

const HexMeshStructuredInfo& HexMesh::structured_info() const
{
    return structured_info_;
}

// =============================
// volume
// =============================
double HexMesh::tetra_volume(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c,
    const MeshNode& d
)
{
    const double ux = b.x - a.x;
    const double uy = b.y - a.y;
    const double uz = b.z - a.z;

    const double vx = c.x - a.x;
    const double vy = c.y - a.y;
    const double vz = c.z - a.z;

    const double wx = d.x - a.x;
    const double wy = d.y - a.y;
    const double wz = d.z - a.z;

    const double det =
        ux * (vy * wz - vz * wy)
      - uy * (vx * wz - vz * wx)
      + uz * (vx * wy - vy * wx);

    return std::abs(det) / 6.0;
}

double HexMesh::hexa_volume(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c,
    const MeshNode& d,
    const MeshNode& e,
    const MeshNode& f,
    const MeshNode& g,
    const MeshNode& h
)
{
    /*
     * 将 Hexa8 拆成 6 个 Tetra4。
     *
     * a b c d = bottom n0 n1 n2 n3
     * e f g h = top    n4 n5 n6 n7
     *
     * 使用主对角线 a -> g。
     */
    return tetra_volume(a, b, c, g)
         + tetra_volume(a, c, d, g)
         + tetra_volume(a, d, h, g)
         + tetra_volume(a, h, e, g)
         + tetra_volume(a, e, f, g)
         + tetra_volume(a, f, b, g);
}

double HexMesh::volume(
    int cell_id
) const
{
    if (!valid_cell_id(cell_id))
    {
        throw std::out_of_range(
            "HexMesh::volume: invalid cell id."
        );
    }

    const auto& c = cells()[static_cast<std::size_t>(cell_id)];

    if (c.type != CellType::Hexa8 || c.node_ids.size() != 8)
    {
        throw std::runtime_error(
            "HexMesh::volume: cell is not Hexa8."
        );
    }

    for (int nid : c.node_ids)
    {
        if (!valid_node_id(nid))
        {
            throw std::runtime_error(
                "HexMesh::volume: invalid node id in cell."
            );
        }
    }

    const auto& n0 = nodes()[static_cast<std::size_t>(c.node_ids[0])];
    const auto& n1 = nodes()[static_cast<std::size_t>(c.node_ids[1])];
    const auto& n2 = nodes()[static_cast<std::size_t>(c.node_ids[2])];
    const auto& n3 = nodes()[static_cast<std::size_t>(c.node_ids[3])];
    const auto& n4 = nodes()[static_cast<std::size_t>(c.node_ids[4])];
    const auto& n5 = nodes()[static_cast<std::size_t>(c.node_ids[5])];
    const auto& n6 = nodes()[static_cast<std::size_t>(c.node_ids[6])];
    const auto& n7 = nodes()[static_cast<std::size_t>(c.node_ids[7])];

    return hexa_volume(n0, n1, n2, n3, n4, n5, n6, n7);
}

// =============================
double HexMesh::total_volume() const
{
    double s = 0.0;

    for (size_t i = 0; i < num_cells(); ++i)
    {
        s += volume(static_cast<int>(i));
    }

    return s;
}

// =============================
std::array<double, 3> HexMesh::centroid(
    int cell_id
) const
{
    if (!valid_cell_id(cell_id))
    {
        throw std::out_of_range(
            "HexMesh::centroid: invalid cell id."
        );
    }

    const auto& c = cells()[static_cast<std::size_t>(cell_id)];

    if (c.type != CellType::Hexa8 || c.node_ids.size() != 8)
    {
        throw std::runtime_error(
            "HexMesh::centroid: cell is not Hexa8."
        );
    }

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    for (int nid : c.node_ids)
    {
        if (!valid_node_id(nid))
        {
            throw std::runtime_error(
                "HexMesh::centroid: invalid node id in cell."
            );
        }

        const auto& p = nodes()[static_cast<std::size_t>(nid)];

        x += p.x;
        y += p.y;
        z += p.z;
    }

    return {
        x / 8.0,
        y / 8.0,
        z / 8.0
    };
}

// =============================
std::array<double, 6> HexMesh::bounds() const
{
    if (num_nodes() == 0)
    {
        return {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }

    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double zmin = std::numeric_limits<double>::max();

    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();
    double zmax = std::numeric_limits<double>::lowest();

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
bool HexMesh::validate(
    std::string* error_message,
    double volume_eps
) const
{
    if (num_nodes() == 0)
    {
        if (error_message)
        {
            *error_message = "HexMesh::validate: mesh has no nodes.";
        }

        return false;
    }

    if (num_cells() == 0)
    {
        if (error_message)
        {
            *error_message = "HexMesh::validate: mesh has no cells.";
        }

        return false;
    }

    for (size_t i = 0; i < num_cells(); ++i)
    {
        const auto& c = cells()[i];

        if (c.type != CellType::Hexa8)
        {
            if (error_message)
            {
                *error_message = "HexMesh::validate: non-Hexa8 cell found.";
            }

            return false;
        }

        if (c.node_ids.size() != 8)
        {
            if (error_message)
            {
                *error_message = "HexMesh::validate: Hexa8 cell must have exactly 8 nodes.";
            }

            return false;
        }

        for (int nid : c.node_ids)
        {
            if (!valid_node_id(nid))
            {
                if (error_message)
                {
                    *error_message = "HexMesh::validate: invalid node id found.";
                }

                return false;
            }
        }

        for (size_t a = 0; a < c.node_ids.size(); ++a)
        {
            for (size_t b = a + 1; b < c.node_ids.size(); ++b)
            {
                if (c.node_ids[a] == c.node_ids[b])
                {
                    if (error_message)
                    {
                        *error_message = "HexMesh::validate: duplicated node id in hexa.";
                    }

                    return false;
                }
            }
        }

        double v = volume(static_cast<int>(i));

        if (v <= volume_eps)
        {
            if (error_message)
            {
                *error_message = "HexMesh::validate: degenerated hexa cell found.";
            }

            return false;
        }
    }

    if (error_message)
    {
        error_message->clear();
    }

    return true;
}

} // namespace OpenCAX
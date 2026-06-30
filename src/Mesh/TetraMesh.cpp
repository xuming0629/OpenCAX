#include <OpenCAX/Mesh/TetraMesh.h>

#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <stdexcept>

namespace OpenCAX
{

int TetraMesh::add_node_3d(
    double x,
    double y,
    double z
)
{
    return add_node(x, y, z);
}

int TetraMesh::add_tetra(
    int n0,
    int n1,
    int n2,
    int n3
)
{
    check_tetra_nodes(
        n0,
        n1,
        n2,
        n3
    );

    return add_cell(
        CellType::Tetra4,
        {n0, n1, n2, n3}
    );
}

int TetraMesh::add_tetra(
    int n0,
    int n1,
    int n2,
    int n3,
    int physical_id,
    int material_id,
    int region_id
)
{
    check_tetra_nodes(
        n0,
        n1,
        n2,
        n3
    );

    return add_cell(
        CellType::Tetra4,
        {n0, n1, n2, n3},
        physical_id,
        material_id,
        region_id
    );
}

std::size_t TetraMesh::num_tetras() const
{
    std::size_t count = 0;

    for (const auto& cell : cells()) {
        if (cell.type == CellType::Tetra4) {
            ++count;
        }
    }

    return count;
}

std::array<int, 4> TetraMesh::tetra(
    std::size_t cell_id
) const
{
    if (cell_id >= cells().size()) {
        throw std::runtime_error(
            "TetraMesh::tetra failed: invalid cell id."
        );
    }

    const MeshCell& cell = cells()[cell_id];

    if (cell.type != CellType::Tetra4 ||
        cell.node_ids.size() != 4) {
        throw std::runtime_error(
            "TetraMesh::tetra failed: invalid Tetra4 cell."
        );
    }

    return {
        cell.node_ids[0],
        cell.node_ids[1],
        cell.node_ids[2],
        cell.node_ids[3]
    };
}

std::array<double, 3> TetraMesh::point3d(
    int node_id
) const
{
    check_node_id(node_id);

    const MeshNode& p = nodes()[node_id];

    return {
        p.x,
        p.y,
        p.z
    };
}

double TetraMesh::signed_tetra_volume(
    std::size_t cell_id
) const
{
    const auto t = tetra(cell_id);

    const MeshNode& p0 = nodes()[t[0]];
    const MeshNode& p1 = nodes()[t[1]];
    const MeshNode& p2 = nodes()[t[2]];
    const MeshNode& p3 = nodes()[t[3]];

    const double ax = p1.x - p0.x;
    const double ay = p1.y - p0.y;
    const double az = p1.z - p0.z;

    const double bx = p2.x - p0.x;
    const double by = p2.y - p0.y;
    const double bz = p2.z - p0.z;

    const double cx = p3.x - p0.x;
    const double cy = p3.y - p0.y;
    const double cz = p3.z - p0.z;

    const double det =
        ax * (by * cz - bz * cy) -
        ay * (bx * cz - bz * cx) +
        az * (bx * cy - by * cx);

    return det / 6.0;
}

double TetraMesh::tetra_volume(
    std::size_t cell_id
) const
{
    return std::abs(
        signed_tetra_volume(cell_id)
    );
}

double TetraMesh::total_volume() const
{
    double volume = 0.0;

    for (std::size_t i = 0; i < cells().size(); ++i) {
        if (cells()[i].type == CellType::Tetra4) {
            volume += tetra_volume(i);
        }
    }

    return volume;
}

std::array<double, 3> TetraMesh::tetra_centroid(
    std::size_t cell_id
) const
{
    const auto t = tetra(cell_id);

    const MeshNode& p0 = nodes()[t[0]];
    const MeshNode& p1 = nodes()[t[1]];
    const MeshNode& p2 = nodes()[t[2]];
    const MeshNode& p3 = nodes()[t[3]];

    return {
        (p0.x + p1.x + p2.x + p3.x) / 4.0,
        (p0.y + p1.y + p2.y + p3.y) / 4.0,
        (p0.z + p1.z + p2.z + p3.z) / 4.0
    };
}

bool TetraMesh::is_pure_tetra_mesh() const
{
    for (const auto& cell : cells()) {
        if (cell.type != CellType::Tetra4 ||
            cell.node_ids.size() != 4) {
            return false;
        }
    }

    return true;
}

std::array<int, 2> TetraMesh::make_edge(
    int a,
    int b
)
{
    if (a < b) {
        return {a, b};
    }

    return {b, a};
}

std::array<int, 3> TetraMesh::make_face(
    int a,
    int b,
    int c
)
{
    std::array<int, 3> f = {a, b, c};
    std::sort(f.begin(), f.end());
    return f;
}

std::vector<std::array<int, 2>> TetraMesh::edges() const
{
    std::set<std::array<int, 2>> edge_set;

    for (const auto& cell : cells()) {
        if (cell.type != CellType::Tetra4 ||
            cell.node_ids.size() != 4) {
            continue;
        }

        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];
        const int n3 = cell.node_ids[3];

        edge_set.insert(make_edge(n0, n1));
        edge_set.insert(make_edge(n1, n2));
        edge_set.insert(make_edge(n2, n0));

        edge_set.insert(make_edge(n0, n3));
        edge_set.insert(make_edge(n1, n3));
        edge_set.insert(make_edge(n2, n3));
    }

    return std::vector<std::array<int, 2>>(
        edge_set.begin(),
        edge_set.end()
    );
}

std::vector<std::array<int, 3>> TetraMesh::faces() const
{
    std::set<std::array<int, 3>> face_set;

    for (const auto& cell : cells()) {
        if (cell.type != CellType::Tetra4 ||
            cell.node_ids.size() != 4) {
            continue;
        }

        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];
        const int n3 = cell.node_ids[3];

        face_set.insert(make_face(n0, n2, n1));
        face_set.insert(make_face(n0, n1, n3));
        face_set.insert(make_face(n1, n2, n3));
        face_set.insert(make_face(n2, n0, n3));
    }

    return std::vector<std::array<int, 3>>(
        face_set.begin(),
        face_set.end()
    );
}

std::vector<std::array<int, 3>> TetraMesh::boundary_faces() const
{
    std::map<std::array<int, 3>, int> face_counter;

    for (const auto& cell : cells()) {
        if (cell.type != CellType::Tetra4 ||
            cell.node_ids.size() != 4) {
            continue;
        }

        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];
        const int n3 = cell.node_ids[3];

        face_counter[make_face(n0, n2, n1)]++;
        face_counter[make_face(n0, n1, n3)]++;
        face_counter[make_face(n1, n2, n3)]++;
        face_counter[make_face(n2, n0, n3)]++;
    }

    std::vector<std::array<int, 3>> result;

    for (const auto& kv : face_counter) {
        if (kv.second == 1) {
            result.push_back(kv.first);
        }
    }

    return result;
}

std::vector<int> TetraMesh::boundary_nodes() const
{
    std::set<int> node_set;

    for (const auto& f : boundary_faces()) {
        node_set.insert(f[0]);
        node_set.insert(f[1]);
        node_set.insert(f[2]);
    }

    return std::vector<int>(
        node_set.begin(),
        node_set.end()
    );
}

TetraMesh TetraMesh::create_unstructured(
    const std::vector<std::array<double, 3>>& points,
    const std::vector<std::array<int, 4>>& tetras
)
{
    if (points.empty()) {
        throw std::runtime_error(
            "TetraMesh::create_unstructured failed: points is empty."
        );
    }

    if (tetras.empty()) {
        throw std::runtime_error(
            "TetraMesh::create_unstructured failed: tetras is empty."
        );
    }

    TetraMesh mesh;

    mesh.info().name = "unstructured_tetra_mesh";
    mesh.info().source = "Unstructured";
    mesh.info().dimension = MeshDimension::Dim3;

    for (const auto& p : points) {
        mesh.add_node_3d(
            p[0],
            p[1],
            p[2]
        );
    }

    for (const auto& t : tetras) {
        mesh.add_tetra(
            t[0],
            t[1],
            t[2],
            t[3]
        );
    }

    return mesh;
}

TetraMesh TetraMesh::create_structured_box(
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
    if (nx <= 0 || ny <= 0 || nz <= 0) {
        throw std::runtime_error(
            "TetraMesh::create_structured_box failed: nx, ny, nz must be positive."
        );
    }

    if (xmax <= xmin || ymax <= ymin || zmax <= zmin) {
        throw std::runtime_error(
            "TetraMesh::create_structured_box failed: invalid bounds."
        );
    }

    TetraMesh mesh;

    mesh.info().name = "structured_tetra_box";
    mesh.info().source = "Structured";
    mesh.info().dimension = MeshDimension::Dim3;

    const double dx =
        (xmax - xmin) / static_cast<double>(nx);

    const double dy =
        (ymax - ymin) / static_cast<double>(ny);

    const double dz =
        (zmax - zmin) / static_cast<double>(nz);

    for (int k = 0; k <= nz; ++k) {
        for (int j = 0; j <= ny; ++j) {
            for (int i = 0; i <= nx; ++i) {
                mesh.add_node_3d(
                    xmin + static_cast<double>(i) * dx,
                    ymin + static_cast<double>(j) * dy,
                    zmin + static_cast<double>(k) * dz
                );
            }
        }
    }

    auto node_id = [nx, ny](int i, int j, int k) -> int {
        return k * (ny + 1) * (nx + 1) +
               j * (nx + 1) +
               i;
    };

    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                const int n000 = node_id(i,     j,     k);
                const int n100 = node_id(i + 1, j,     k);
                const int n010 = node_id(i,     j + 1, k);
                const int n110 = node_id(i + 1, j + 1, k);

                const int n001 = node_id(i,     j,     k + 1);
                const int n101 = node_id(i + 1, j,     k + 1);
                const int n011 = node_id(i,     j + 1, k + 1);
                const int n111 = node_id(i + 1, j + 1, k + 1);

                /*
                 * 六面体剖分为 6 个四面体。
                 *
                 * 该剖分沿主对角线 n000 -> n111。
                 */
                mesh.add_tetra(n000, n100, n110, n111);
                mesh.add_tetra(n000, n110, n010, n111);
                mesh.add_tetra(n000, n010, n011, n111);
                mesh.add_tetra(n000, n011, n001, n111);
                mesh.add_tetra(n000, n001, n101, n111);
                mesh.add_tetra(n000, n101, n100, n111);
            }
        }
    }

    return mesh;
}

void TetraMesh::check_node_id(
    int node_id
) const
{
    if (node_id < 0 ||
        node_id >= static_cast<int>(nodes().size())) {
        throw std::runtime_error(
            "TetraMesh failed: invalid node id."
        );
    }
}

void TetraMesh::check_tetra_nodes(
    int n0,
    int n1,
    int n2,
    int n3
) const
{
    check_node_id(n0);
    check_node_id(n1);
    check_node_id(n2);
    check_node_id(n3);

    if (n0 == n1 || n0 == n2 || n0 == n3 ||
        n1 == n2 || n1 == n3 ||
        n2 == n3) {
        throw std::runtime_error(
            "TetraMesh failed: duplicated tetra node id."
        );
    }

    const MeshNode& p0 = nodes()[n0];
    const MeshNode& p1 = nodes()[n1];
    const MeshNode& p2 = nodes()[n2];
    const MeshNode& p3 = nodes()[n3];

    const double ax = p1.x - p0.x;
    const double ay = p1.y - p0.y;
    const double az = p1.z - p0.z;

    const double bx = p2.x - p0.x;
    const double by = p2.y - p0.y;
    const double bz = p2.z - p0.z;

    const double cx = p3.x - p0.x;
    const double cy = p3.y - p0.y;
    const double cz = p3.z - p0.z;

    const double det =
        ax * (by * cz - bz * cy) -
        ay * (bx * cz - bz * cx) +
        az * (bx * cy - by * cx);

    if (std::abs(det) < 1.0e-15) {
        throw std::runtime_error(
            "TetraMesh failed: degenerated tetra."
        );
    }
}

} // namespace OpenCAX
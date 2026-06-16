#include <OpenCAX/Mesh/TriangleMesh.h>

#include <cmath>
#include <map>
#include <set>
#include <stdexcept>

namespace OpenCAX
{

int TriangleMesh::add_node_2d(
    double x,
    double y
)
{
    return add_node(x, y, 0.0);
}

int TriangleMesh::add_triangle(
    int n0,
    int n1,
    int n2
)
{
    check_triangle_nodes(n0, n1, n2);

    return add_cell(
        CellType::Triangle3,
        {n0, n1, n2}
    );
}

int TriangleMesh::add_triangle(
    int n0,
    int n1,
    int n2,
    int physical_id,
    int material_id,
    int region_id
)
{
    check_triangle_nodes(n0, n1, n2);

    return add_cell(
        CellType::Triangle3,
        {n0, n1, n2},
        physical_id,
        material_id,
        region_id
    );
}

std::size_t TriangleMesh::num_triangles() const
{
    std::size_t count = 0;

    for (const auto& cell : cells()) {
        if (cell.type == CellType::Triangle3) {
            ++count;
        }
    }

    return count;
}

std::array<int, 3> TriangleMesh::triangle(
    std::size_t cell_id
) const
{
    if (cell_id >= cells().size()) {
        throw std::runtime_error(
            "TriangleMesh::triangle failed: invalid cell id."
        );
    }

    const MeshCell& cell = cells()[cell_id];

    if (cell.type != CellType::Triangle3 ||
        cell.node_ids.size() != 3) {
        throw std::runtime_error(
            "TriangleMesh::triangle failed: invalid Triangle3 cell."
        );
    }

    return {
        cell.node_ids[0],
        cell.node_ids[1],
        cell.node_ids[2]
    };
}

std::array<double, 2> TriangleMesh::point2d(
    int node_id
) const
{
    check_node_id(node_id);

    const MeshNode& p = nodes()[node_id];

    return {p.x, p.y};
}

double TriangleMesh::signed_triangle_area(
    std::size_t cell_id
) const
{
    const auto tri = triangle(cell_id);

    const MeshNode& p0 = nodes()[tri[0]];
    const MeshNode& p1 = nodes()[tri[1]];
    const MeshNode& p2 = nodes()[tri[2]];

    const double x1 = p1.x - p0.x;
    const double y1 = p1.y - p0.y;

    const double x2 = p2.x - p0.x;
    const double y2 = p2.y - p0.y;

    return 0.5 * (x1 * y2 - y1 * x2);
}

double TriangleMesh::triangle_area(
    std::size_t cell_id
) const
{
    return std::abs(
        signed_triangle_area(cell_id)
    );
}

double TriangleMesh::total_area() const
{
    double area = 0.0;

    for (std::size_t i = 0; i < cells().size(); ++i) {
        if (cells()[i].type == CellType::Triangle3) {
            area += triangle_area(i);
        }
    }

    return area;
}

std::array<double, 2> TriangleMesh::triangle_centroid(
    std::size_t cell_id
) const
{
    const auto tri = triangle(cell_id);

    const MeshNode& p0 = nodes()[tri[0]];
    const MeshNode& p1 = nodes()[tri[1]];
    const MeshNode& p2 = nodes()[tri[2]];

    return {
        (p0.x + p1.x + p2.x) / 3.0,
        (p0.y + p1.y + p2.y) / 3.0
    };
}

bool TriangleMesh::is_pure_triangle_mesh() const
{
    for (const auto& cell : cells()) {
        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3) {
            return false;
        }
    }

    return true;
}

std::array<int, 2> TriangleMesh::make_edge(
    int a,
    int b
)
{
    if (a < b) {
        return {a, b};
    }

    return {b, a};
}

std::vector<std::array<int, 2>> TriangleMesh::edges() const
{
    std::set<std::array<int, 2>> edge_set;

    for (const auto& cell : cells()) {
        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3) {
            continue;
        }

        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];

        edge_set.insert(make_edge(n0, n1));
        edge_set.insert(make_edge(n1, n2));
        edge_set.insert(make_edge(n2, n0));
    }

    return std::vector<std::array<int, 2>>(
        edge_set.begin(),
        edge_set.end()
    );
}

std::vector<std::array<int, 2>> TriangleMesh::boundary_edges() const
{
    std::map<std::array<int, 2>, int> edge_counter;

    for (const auto& cell : cells()) {
        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3) {
            continue;
        }

        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];

        edge_counter[make_edge(n0, n1)]++;
        edge_counter[make_edge(n1, n2)]++;
        edge_counter[make_edge(n2, n0)]++;
    }

    std::vector<std::array<int, 2>> result;

    for (const auto& kv : edge_counter) {
        if (kv.second == 1) {
            result.push_back(kv.first);
        }
    }

    return result;
}

std::vector<int> TriangleMesh::boundary_nodes() const
{
    std::set<int> node_set;

    for (const auto& e : boundary_edges()) {
        node_set.insert(e[0]);
        node_set.insert(e[1]);
    }

    return std::vector<int>(
        node_set.begin(),
        node_set.end()
    );
}

TriangleMesh TriangleMesh::create_structured_rectangle(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    int nx,
    int ny
)
{
    if (nx <= 0 || ny <= 0) {
        throw std::runtime_error(
            "TriangleMesh::create_structured_rectangle failed: nx and ny must be positive."
        );
    }

    if (xmax <= xmin || ymax <= ymin) {
        throw std::runtime_error(
            "TriangleMesh::create_structured_rectangle failed: invalid bounds."
        );
    }

    TriangleMesh mesh;

    mesh.info().name = "structured_triangle_rectangle";
    mesh.info().source = "Structured";
    mesh.info().dimension = MeshDimension::Dim2;

    const double dx =
        (xmax - xmin) / static_cast<double>(nx);

    const double dy =
        (ymax - ymin) / static_cast<double>(ny);

    for (int j = 0; j <= ny; ++j) {
        for (int i = 0; i <= nx; ++i) {
            mesh.add_node_2d(
                xmin + static_cast<double>(i) * dx,
                ymin + static_cast<double>(j) * dy
            );
        }
    }

    auto node_id = [nx](int i, int j) -> int {
        return j * (nx + 1) + i;
    };

    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            const int n00 = node_id(i,     j);
            const int n10 = node_id(i + 1, j);
            const int n01 = node_id(i,     j + 1);
            const int n11 = node_id(i + 1, j + 1);

            mesh.add_triangle(n00, n10, n11);
            mesh.add_triangle(n00, n11, n01);
        }
    }

    return mesh;
}

TriangleMesh TriangleMesh::create_unstructured(
    const std::vector<std::array<double, 2>>& points,
    const std::vector<std::array<int, 3>>& triangles
)
{
    if (points.empty()) {
        throw std::runtime_error(
            "TriangleMesh::create_unstructured failed: points is empty."
        );
    }

    if (triangles.empty()) {
        throw std::runtime_error(
            "TriangleMesh::create_unstructured failed: triangles is empty."
        );
    }

    TriangleMesh mesh;

    mesh.info().name = "unstructured_triangle_mesh";
    mesh.info().source = "Unstructured";
    mesh.info().dimension = MeshDimension::Dim2;

    for (const auto& p : points) {
        mesh.add_node_2d(
            p[0],
            p[1]
        );
    }

    for (const auto& tri : triangles) {
        mesh.add_triangle(
            tri[0],
            tri[1],
            tri[2]
        );
    }

    return mesh;
}

void TriangleMesh::check_node_id(
    int node_id
) const
{
    if (node_id < 0 ||
        node_id >= static_cast<int>(nodes().size())) {
        throw std::runtime_error(
            "TriangleMesh failed: invalid node id."
        );
    }
}

void TriangleMesh::check_triangle_nodes(
    int n0,
    int n1,
    int n2
) const
{
    check_node_id(n0);
    check_node_id(n1);
    check_node_id(n2);

    if (n0 == n1 || n1 == n2 || n2 == n0) {
        throw std::runtime_error(
            "TriangleMesh failed: duplicated triangle node id."
        );
    }

    const MeshNode& p0 = nodes()[n0];
    const MeshNode& p1 = nodes()[n1];
    const MeshNode& p2 = nodes()[n2];

    const double x1 = p1.x - p0.x;
    const double y1 = p1.y - p0.y;

    const double x2 = p2.x - p0.x;
    const double y2 = p2.y - p0.y;

    const double area2 = x1 * y2 - y1 * x2;

    if (std::abs(area2) < 1.0e-15) {
        throw std::runtime_error(
            "TriangleMesh failed: degenerated triangle."
        );
    }
}

} // namespace OpenCAX
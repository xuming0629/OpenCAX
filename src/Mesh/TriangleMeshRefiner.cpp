/**
 * @file TriangleMeshRefiner.cpp
 * @brief OpenCAX 三角形网格加密工具实现
 */

#include <OpenCAX/Mesh/TriangleMeshRefiner.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace OpenCAX
{

namespace
{

/**
 * @brief 无向边 key
 */
struct EdgeKey
{
    int a = -1;
    int b = -1;

    EdgeKey() = default;

    EdgeKey(
        int n0,
        int n1
    )
    {
        if (n0 < n1)
        {
            a = n0;
            b = n1;
        }
        else
        {
            a = n1;
            b = n0;
        }
    }

    bool operator==(
        const EdgeKey& other
    ) const
    {
        return a == other.a &&
               b == other.b;
    }
};

/**
 * @brief EdgeKey hash
 */
struct EdgeKeyHash
{
    std::size_t operator()(
        const EdgeKey& e
    ) const
    {
        const std::size_t h1 =
            std::hash<int>()(e.a);

        const std::size_t h2 =
            std::hash<int>()(e.b);

        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

using EdgeSet =
    std::unordered_set<EdgeKey, EdgeKeyHash>;

using EdgeMidpointMap =
    std::unordered_map<EdgeKey, int, EdgeKeyHash>;

/**
 * @brief 检查三角形网格
 */
void check_triangle_mesh_input(
    const TriangleMesh& mesh
)
{
    if (mesh.num_nodes() == 0)
    {
        throw std::invalid_argument(
            "TriangleMeshRefiner: input mesh has no nodes."
        );
    }

    if (mesh.num_cells() == 0)
    {
        throw std::invalid_argument(
            "TriangleMeshRefiner: input mesh has no cells."
        );
    }

    for (const auto& cell : mesh.cells())
    {
        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3)
        {
            throw std::invalid_argument(
                "TriangleMeshRefiner: input mesh must contain Triangle3 cells only."
            );
        }

        for (int node_id : cell.node_ids)
        {
            if (!mesh.valid_node_id(node_id))
            {
                throw std::invalid_argument(
                    "TriangleMeshRefiner: invalid node id in input mesh."
                );
            }
        }
    }
}

/**
 * @brief 两点距离平方
 */
double distance2(
    const MeshNode& a,
    const MeshNode& b
)
{
    const double dx = a.x - b.x;
    const double dy = a.y - b.y;
    const double dz = a.z - b.z;

    return dx * dx + dy * dy + dz * dz;
}

/**
 * @brief 判断三角形最长边
 */
EdgeKey longest_edge_of_cell(
    const TriangleMesh& mesh,
    const MeshCell& cell
)
{
    const auto& nodes = mesh.nodes();

    const int n0 = cell.node_ids[0];
    const int n1 = cell.node_ids[1];
    const int n2 = cell.node_ids[2];

    const auto& p0 = nodes[static_cast<std::size_t>(n0)];
    const auto& p1 = nodes[static_cast<std::size_t>(n1)];
    const auto& p2 = nodes[static_cast<std::size_t>(n2)];

    const double d01 = distance2(p0, p1);
    const double d12 = distance2(p1, p2);
    const double d20 = distance2(p2, p0);

    if (d01 >= d12 && d01 >= d20)
    {
        return EdgeKey(n0, n1);
    }

    if (d12 >= d01 && d12 >= d20)
    {
        return EdgeKey(n1, n2);
    }

    return EdgeKey(n2, n0);
}

/**
 * @brief 计算 XY 平面有向面积的 2 倍
 */
double signed_area2_xy(
    const std::vector<std::array<double, 3>>& points,
    int n0,
    int n1,
    int n2
)
{
    const auto& a = points[static_cast<std::size_t>(n0)];
    const auto& b = points[static_cast<std::size_t>(n1)];
    const auto& c = points[static_cast<std::size_t>(n2)];

    return (b[0] - a[0]) * (c[1] - a[1]) -
           (b[1] - a[1]) * (c[0] - a[0]);
}

/**
 * @brief 添加 CCW 三角形
 */
void add_triangle_ccw(
    const std::vector<std::array<double, 3>>& points,
    std::vector<std::array<int, 3>>& triangles,
    int n0,
    int n1,
    int n2
)
{
    const double s =
        signed_area2_xy(
            points,
            n0,
            n1,
            n2
        );

    if (s >= 0.0)
    {
        triangles.push_back({n0, n1, n2});
    }
    else
    {
        triangles.push_back({n0, n2, n1});
    }
}

/**
 * @brief 从输入网格拷贝节点坐标
 */
std::vector<std::array<double, 3>> copy_points(
    const TriangleMesh& mesh
)
{
    std::vector<std::array<double, 3>> points;
    points.reserve(mesh.nodes().size());

    for (const auto& node : mesh.nodes())
    {
        points.push_back({
            node.x,
            node.y,
            node.z
        });
    }

    return points;
}

/**
 * @brief 获取边中点节点 ID
 *
 * 如果该边中点已经存在，直接返回已有 ID。
 * 如果不存在，则创建新节点。
 */
int get_or_create_midpoint(
    const EdgeKey& edge,
    std::vector<std::array<double, 3>>& points,
    EdgeMidpointMap& midpoint_map
)
{
    const auto it =
        midpoint_map.find(edge);

    if (it != midpoint_map.end())
    {
        return it->second;
    }

    const auto& p0 =
        points[static_cast<std::size_t>(edge.a)];

    const auto& p1 =
        points[static_cast<std::size_t>(edge.b)];

    const std::array<double, 3> mid = {
        0.5 * (p0[0] + p1[0]),
        0.5 * (p0[1] + p1[1]),
        0.5 * (p0[2] + p1[2])
    };

    const int mid_id =
        static_cast<int>(points.size());

    points.push_back(mid);

    midpoint_map[edge] = mid_id;

    return mid_id;
}

/**
 * @brief 根据需要切分的边集合，加密整个三角形网格
 *
 * 这是核心函数。
 *
 * marked_edges 是全局边集合。
 *
 * 每个 cell 会检查自己的三条边：
 *
 * - 0 条边被切分：保持原三角形
 * - 1 条边被切分：一分二
 * - 2 条边被切分：一分三
 * - 3 条边被切分：一分四
 */
TriangleMesh refine_by_marked_edges(
    const TriangleMesh& mesh,
    const EdgeSet& marked_edges
)
{
    check_triangle_mesh_input(mesh);

    std::vector<std::array<double, 3>> points =
        copy_points(mesh);

    std::vector<std::array<int, 3>> triangles;

    triangles.reserve(
        mesh.cells().size() * 4
    );

    EdgeMidpointMap midpoint_map;

    for (const auto& cell : mesh.cells())
    {
        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];

        const EdgeKey e01(n0, n1);
        const EdgeKey e12(n1, n2);
        const EdgeKey e20(n2, n0);

        const bool s01 =
            marked_edges.find(e01) != marked_edges.end();

        const bool s12 =
            marked_edges.find(e12) != marked_edges.end();

        const bool s20 =
            marked_edges.find(e20) != marked_edges.end();

        const int split_count =
            static_cast<int>(s01) +
            static_cast<int>(s12) +
            static_cast<int>(s20);

        if (split_count == 0)
        {
            add_triangle_ccw(
                points,
                triangles,
                n0,
                n1,
                n2
            );

            continue;
        }

        int m01 = -1;
        int m12 = -1;
        int m20 = -1;

        if (s01)
        {
            m01 = get_or_create_midpoint(
                e01,
                points,
                midpoint_map
            );
        }

        if (s12)
        {
            m12 = get_or_create_midpoint(
                e12,
                points,
                midpoint_map
            );
        }

        if (s20)
        {
            m20 = get_or_create_midpoint(
                e20,
                points,
                midpoint_map
            );
        }

        /*
         * 情况 1：只有一条边被切分，一分二。
         */
        if (split_count == 1)
        {
            if (s01)
            {
                add_triangle_ccw(points, triangles, n0, m01, n2);
                add_triangle_ccw(points, triangles, m01, n1, n2);
            }
            else if (s12)
            {
                add_triangle_ccw(points, triangles, n1, m12, n0);
                add_triangle_ccw(points, triangles, m12, n2, n0);
            }
            else
            {
                add_triangle_ccw(points, triangles, n2, m20, n1);
                add_triangle_ccw(points, triangles, m20, n0, n1);
            }

            continue;
        }

        /*
         * 情况 2：两条边被切分，一分三。
         *
         * 两条被切分的边必然共享一个顶点。
         */
        if (split_count == 2)
        {
            if (s01 && s12)
            {
                /*
                 * split edges: n0-n1, n1-n2
                 * shared vertex: n1
                 */
                add_triangle_ccw(points, triangles, n1, m12, m01);
                add_triangle_ccw(points, triangles, n0, m01, n2);
                add_triangle_ccw(points, triangles, m01, m12, n2);
            }
            else if (s12 && s20)
            {
                /*
                 * split edges: n1-n2, n2-n0
                 * shared vertex: n2
                 */
                add_triangle_ccw(points, triangles, n2, m20, m12);
                add_triangle_ccw(points, triangles, n1, m12, n0);
                add_triangle_ccw(points, triangles, m12, m20, n0);
            }
            else
            {
                /*
                 * split edges: n2-n0, n0-n1
                 * shared vertex: n0
                 */
                add_triangle_ccw(points, triangles, n0, m01, m20);
                add_triangle_ccw(points, triangles, n2, m20, n1);
                add_triangle_ccw(points, triangles, m20, m01, n1);
            }

            continue;
        }

        /*
         * 情况 3：三条边都被切分，一分四。
         */
        add_triangle_ccw(points, triangles, n0, m01, m20);
        add_triangle_ccw(points, triangles, m01, n1, m12);
        add_triangle_ccw(points, triangles, m20, m12, n2);
        add_triangle_ccw(points, triangles, m01, m12, m20);
    }

    TriangleMesh refined =
        TriangleMesh::create_unstructured(
            points,
            triangles
        );

    refined.set_name("RefinedTriangleMesh");
    refined.set_source("OpenCAX::TriangleMeshRefiner");

    return refined;
}

/**
 * @brief 标记所有三角形的所有边
 *
 * 用于一致加密。
 */
EdgeSet collect_all_edges(
    const TriangleMesh& mesh
)
{
    EdgeSet edges;

    for (const auto& cell : mesh.cells())
    {
        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];

        edges.insert(EdgeKey(n0, n1));
        edges.insert(EdgeKey(n1, n2));
        edges.insert(EdgeKey(n2, n0));
    }

    return edges;
}

/**
 * @brief 标记所有 cell 的最长边
 *
 * 用于全局二分加密。
 */
EdgeSet collect_longest_edges_of_all_cells(
    const TriangleMesh& mesh
)
{
    EdgeSet edges;

    for (const auto& cell : mesh.cells())
    {
        edges.insert(
            longest_edge_of_cell(mesh, cell)
        );
    }

    return edges;
}

/**
 * @brief 标记指定 cell 的最长边
 *
 * 用于局部二分加密。
 */
EdgeSet collect_longest_edges_of_marked_cells(
    const TriangleMesh& mesh,
    const std::vector<int>& marked_cell_ids
)
{
    EdgeSet edges;

    for (int cell_id : marked_cell_ids)
    {
        if (!mesh.valid_cell_id(cell_id))
        {
            throw std::out_of_range(
                "TriangleMeshRefiner::bisection_refine_marked_cells: invalid cell id."
            );
        }

        const auto& cell =
            mesh.cells()[static_cast<std::size_t>(cell_id)];

        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3)
        {
            throw std::invalid_argument(
                "TriangleMeshRefiner::bisection_refine_marked_cells: marked cell is not Triangle3."
            );
        }

        edges.insert(
            longest_edge_of_cell(mesh, cell)
        );
    }

    return edges;
}

} // namespace

TriangleMesh TriangleMeshRefiner::uniform_refine(
    const TriangleMesh& mesh,
    int levels
)
{
    if (levels <= 0)
    {
        return mesh;
    }

    TriangleMesh current = mesh;

    for (int level = 0; level < levels; ++level)
    {
        const EdgeSet edges =
            collect_all_edges(current);

        current =
            refine_by_marked_edges(
                current,
                edges
            );

        current.set_name("UniformRefinedTriangleMesh");
        current.set_source("OpenCAX::TriangleMeshRefiner::uniform_refine");
    }

    return current;
}

TriangleMesh TriangleMeshRefiner::bisection_refine(
    const TriangleMesh& mesh,
    int levels
)
{
    if (levels <= 0)
    {
        return mesh;
    }

    TriangleMesh current = mesh;

    for (int level = 0; level < levels; ++level)
    {
        const EdgeSet edges =
            collect_longest_edges_of_all_cells(current);

        current =
            refine_by_marked_edges(
                current,
                edges
            );

        current.set_name("BisectionRefinedTriangleMesh");
        current.set_source("OpenCAX::TriangleMeshRefiner::bisection_refine");
    }

    return current;
}

TriangleMesh TriangleMeshRefiner::bisection_refine_marked_cells(
    const TriangleMesh& mesh,
    const std::vector<int>& marked_cell_ids
)
{
    if (marked_cell_ids.empty())
    {
        return mesh;
    }

    const EdgeSet edges =
        collect_longest_edges_of_marked_cells(
            mesh,
            marked_cell_ids
        );

    TriangleMesh refined =
        refine_by_marked_edges(
            mesh,
            edges
        );

    refined.set_name("MarkedBisectionRefinedTriangleMesh");
    refined.set_source("OpenCAX::TriangleMeshRefiner::bisection_refine_marked_cells");

    return refined;
}

} // namespace OpenCAX
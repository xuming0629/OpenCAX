/**
 * @file TriangleMesh.cpp
 * @brief OpenCAX 二维三角形网格类实现
 *
 * 本文件实现 TriangleMesh 类。
 *
 * 主要功能包括：
 *
 * - 结构矩形区域三角网格生成
 * - 非结构三角网格构造
 * - 三角形面积计算
 * - 总面积计算
 * - 单元质心计算
 * - 包围盒计算
 * - 三角形网格合法性检查
 */

#include <OpenCAX/Mesh/TriangleMesh.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace OpenCAX
{

namespace
{

/**
 * @brief 计算三角形在 XY 平面上的有向面积的 2 倍
 *
 * 如果返回值大于 0，说明节点顺序在 XY 平面上是逆时针。
 * 如果返回值小于 0，说明节点顺序在 XY 平面上是顺时针。
 *
 * @param a 第一个节点
 * @param b 第二个节点
 * @param c 第三个节点
 * @return XY 平面有向面积的 2 倍
 */
double signed_area2_xy(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    return (b.x - a.x) * (c.y - a.y) -
           (b.y - a.y) * (c.x - a.x);
}

/**
 * @brief 将三角形节点顺序调整为 XY 平面逆时针
 *
 * @param mesh 网格
 * @param n0 第一个节点 ID
 * @param n1 第二个节点 ID
 * @param n2 第三个节点 ID
 * @return 逆时针节点顺序
 *
 * @note
 * 如果三角形在 XY 平面退化，则保留原始顺序。
 */
std::array<int, 3> make_ccw_xy(
    const TriangleMesh& mesh,
    int n0,
    int n1,
    int n2
)
{
    const auto& nodes = mesh.nodes();

    const MeshNode& a = nodes[static_cast<std::size_t>(n0)];
    const MeshNode& b = nodes[static_cast<std::size_t>(n1)];
    const MeshNode& c = nodes[static_cast<std::size_t>(n2)];

    const double s = signed_area2_xy(a, b, c);

    if (s < 0.0)
    {
        return {n0, n2, n1};
    }

    return {n0, n1, n2};
}

} // namespace

TriangleMesh::TriangleMesh()
{
    info_.dimension = MeshDimension::Dim2;
    info_.source = "TriangleMesh";
}

TriangleMesh TriangleMesh::create_structured_rectangle(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    int nx,
    int ny,
    bool alternate_diagonal
)
{
    if (nx <= 0)
    {
        throw std::invalid_argument(
            "TriangleMesh::create_structured_rectangle: nx must be positive."
        );
    }

    if (ny <= 0)
    {
        throw std::invalid_argument(
            "TriangleMesh::create_structured_rectangle: ny must be positive."
        );
    }

    if (!(xmax > xmin))
    {
        throw std::invalid_argument(
            "TriangleMesh::create_structured_rectangle: xmax must be greater than xmin."
        );
    }

    if (!(ymax > ymin))
    {
        throw std::invalid_argument(
            "TriangleMesh::create_structured_rectangle: ymax must be greater than ymin."
        );
    }

    TriangleMesh mesh;

    mesh.info_.name = "StructuredTriangleMesh";
    mesh.info_.source = "Structured";
    mesh.info_.dimension = MeshDimension::Dim2;

    mesh.source_type_ = TriangleMeshSourceType::Structured;
    mesh.structured_info_.nx = nx;
    mesh.structured_info_.ny = ny;
    mesh.structured_info_.xmin = xmin;
    mesh.structured_info_.xmax = xmax;
    mesh.structured_info_.ymin = ymin;
    mesh.structured_info_.ymax = ymax;
    mesh.structured_info_.alternate_diagonal = alternate_diagonal;

    const double dx = (xmax - xmin) / static_cast<double>(nx);
    const double dy = (ymax - ymin) / static_cast<double>(ny);

    auto node_index = [nx](int i, int j) -> int
    {
        return j * (nx + 1) + i;
    };

    /*
     * 生成节点。
     *
     * 节点编号规则：
     *
     * j = ny:  ...
     *          6 --- 7 --- 8
     *          |     |     |
     *          3 --- 4 --- 5
     *          |     |     |
     * j = 0:   0 --- 1 --- 2
     *
     * 即：
     *
     * node_id(i, j) = j * (nx + 1) + i
     */
    for (int j = 0; j <= ny; ++j)
    {
        const double y = ymin + static_cast<double>(j) * dy;

        for (int i = 0; i <= nx; ++i)
        {
            const double x = xmin + static_cast<double>(i) * dx;
            mesh.add_node(x, y, 0.0);
        }
    }

    /*
     * 生成 Triangle3 单元。
     *
     * 重要约定：
     *
     * 1. 每个三角形节点按逆时针方向排列。
     * 2. 每个直角三角形的直角点放在 node_ids[1]。
     *
     * 对于一个矩形：
     *
     * n01 ----- n11
     *  |         |
     *  |         |
     * n00 ----- n10
     *
     * 默认对角线 n00 -> n11：
     *
     * - {n00, n10, n11}
     *   逆时针，直角点 n10 在中间。
     *
     * - {n11, n01, n00}
     *   逆时针，直角点 n01 在中间。
     *
     * 交错对角线 n10 -> n01：
     *
     * - {n01, n00, n10}
     *   逆时针，直角点 n00 在中间。
     *
     * - {n10, n11, n01}
     *   逆时针，直角点 n11 在中间。
     */
    for (int j = 0; j < ny; ++j)
    {
        for (int i = 0; i < nx; ++i)
        {
            const int n00 = node_index(i, j);
            const int n10 = node_index(i + 1, j);
            const int n01 = node_index(i, j + 1);
            const int n11 = node_index(i + 1, j + 1);

            const bool flip = alternate_diagonal && ((i + j) % 2 == 1);

            if (!flip)
            {
                /*
                 * 对角线 n00 -> n11。
                 */
                mesh.add_cell(
                    CellType::Triangle3,
                    {n00, n10, n11}
                );

                mesh.add_cell(
                    CellType::Triangle3,
                    {n11, n01, n00}
                );
            }
            else
            {
                /*
                 * 对角线 n10 -> n01。
                 */
                mesh.add_cell(
                    CellType::Triangle3,
                    {n01, n00, n10}
                );

                mesh.add_cell(
                    CellType::Triangle3,
                    {n10, n11, n01}
                );
            }
        }
    }

    return mesh;
}

TriangleMesh TriangleMesh::create_unstructured(
    const std::vector<std::array<double, 3>>& points,
    const std::vector<std::array<int, 3>>& triangles
)
{
    TriangleMesh mesh;

    mesh.info_.name = "UnstructuredTriangleMesh";
    mesh.info_.source = "Unstructured";
    mesh.info_.dimension = MeshDimension::Dim2;

    mesh.source_type_ = TriangleMeshSourceType::Unstructured;

    for (const auto& p : points)
    {
        mesh.add_node(p[0], p[1], p[2]);
    }

    /*
     * 非结构三角形网格构造。
     *
     * 对输入三角形做一个轻量处理：
     *
     * - 如果三角形在 XY 平面投影下是顺时针，则交换后两个节点，使其变成逆时针。
     * - 如果三角形在 XY 平面投影下退化，则保持原始输入顺序。
     *
     * @note
     * 对一般三维曲面网格，严格的方向约定应基于目标法向。
     * 当前 TriangleMesh 主要服务于二维平面三角网格，因此采用 XY 平面判断。
     */
    for (const auto& tri : triangles)
    {
        if (!mesh.valid_node_id(tri[0]) ||
            !mesh.valid_node_id(tri[1]) ||
            !mesh.valid_node_id(tri[2]))
        {
            throw std::out_of_range(
                "TriangleMesh::create_unstructured: invalid node id in triangle."
            );
        }

        const auto ccw = make_ccw_xy(
            mesh,
            tri[0],
            tri[1],
            tri[2]
        );

        mesh.add_cell(
            CellType::Triangle3,
            {ccw[0], ccw[1], ccw[2]}
        );
    }

    return mesh;
}

TriangleMeshSourceType TriangleMesh::source_type() const
{
    return source_type_;
}

void TriangleMesh::set_source_type(
    TriangleMeshSourceType type
)
{
    source_type_ = type;
}

bool TriangleMesh::is_structured() const
{
    return source_type_ == TriangleMeshSourceType::Structured;
}

bool TriangleMesh::is_unstructured() const
{
    return source_type_ == TriangleMeshSourceType::Unstructured;
}

const TriangleMeshStructuredInfo& TriangleMesh::structured_info() const
{
    return structured_info_;
}

double TriangleMesh::triangle_area(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    const double abx = b.x - a.x;
    const double aby = b.y - a.y;
    const double abz = b.z - a.z;

    const double acx = c.x - a.x;
    const double acy = c.y - a.y;
    const double acz = c.z - a.z;

    const double cx = aby * acz - abz * acy;
    const double cy = abz * acx - abx * acz;
    const double cz = abx * acy - aby * acx;

    return 0.5 * std::sqrt(cx * cx + cy * cy + cz * cz);
}

double TriangleMesh::area(
    int cell_id
) const
{
    if (!valid_cell_id(cell_id))
    {
        throw std::out_of_range(
            "TriangleMesh::area: invalid cell id."
        );
    }

    const MeshCell& cell = cells_[static_cast<std::size_t>(cell_id)];

    if (cell.type != CellType::Triangle3 || cell.node_ids.size() != 3)
    {
        throw std::runtime_error(
            "TriangleMesh::area: cell is not Triangle3."
        );
    }

    const int n0 = cell.node_ids[0];
    const int n1 = cell.node_ids[1];
    const int n2 = cell.node_ids[2];

    if (!valid_node_id(n0) ||
        !valid_node_id(n1) ||
        !valid_node_id(n2))
    {
        throw std::runtime_error(
            "TriangleMesh::area: invalid node id in cell."
        );
    }

    return triangle_area(
        nodes_[static_cast<std::size_t>(n0)],
        nodes_[static_cast<std::size_t>(n1)],
        nodes_[static_cast<std::size_t>(n2)]
    );
}

double TriangleMesh::total_area() const
{
    double result = 0.0;

    for (std::size_t i = 0; i < cells_.size(); ++i)
    {
        result += area(static_cast<int>(i));
    }

    return result;
}

std::array<double, 3> TriangleMesh::centroid(
    int cell_id
) const
{
    if (!valid_cell_id(cell_id))
    {
        throw std::out_of_range(
            "TriangleMesh::centroid: invalid cell id."
        );
    }

    const MeshCell& cell = cells_[static_cast<std::size_t>(cell_id)];

    if (cell.type != CellType::Triangle3 || cell.node_ids.size() != 3)
    {
        throw std::runtime_error(
            "TriangleMesh::centroid: cell is not Triangle3."
        );
    }

    const int n0 = cell.node_ids[0];
    const int n1 = cell.node_ids[1];
    const int n2 = cell.node_ids[2];

    if (!valid_node_id(n0) ||
        !valid_node_id(n1) ||
        !valid_node_id(n2))
    {
        throw std::runtime_error(
            "TriangleMesh::centroid: invalid node id in cell."
        );
    }

    const MeshNode& a = nodes_[static_cast<std::size_t>(n0)];
    const MeshNode& b = nodes_[static_cast<std::size_t>(n1)];
    const MeshNode& c = nodes_[static_cast<std::size_t>(n2)];

    return {
        (a.x + b.x + c.x) / 3.0,
        (a.y + b.y + c.y) / 3.0,
        (a.z + b.z + c.z) / 3.0
    };
}

std::array<double, 6> TriangleMesh::bounds() const
{
    if (nodes_.empty())
    {
        return {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }

    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double zmin = std::numeric_limits<double>::max();

    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();
    double zmax = std::numeric_limits<double>::lowest();

    for (const auto& node : nodes_)
    {
        xmin = std::min(xmin, node.x);
        xmax = std::max(xmax, node.x);

        ymin = std::min(ymin, node.y);
        ymax = std::max(ymax, node.y);

        zmin = std::min(zmin, node.z);
        zmax = std::max(zmax, node.z);
    }

    return {xmin, xmax, ymin, ymax, zmin, zmax};
}

bool TriangleMesh::validate(
    std::string* error_message,
    double area_eps
) const
{
    if (nodes_.empty())
    {
        if (error_message)
        {
            *error_message = "TriangleMesh::validate: mesh has no nodes.";
        }

        return false;
    }

    if (cells_.empty())
    {
        if (error_message)
        {
            *error_message = "TriangleMesh::validate: mesh has no cells.";
        }

        return false;
    }

    for (std::size_t ci = 0; ci < cells_.size(); ++ci)
    {
        const MeshCell& cell = cells_[ci];

        if (cell.type != CellType::Triangle3)
        {
            if (error_message)
            {
                *error_message = "TriangleMesh::validate: non-Triangle3 cell found.";
            }

            return false;
        }

        if (cell.node_ids.size() != 3)
        {
            if (error_message)
            {
                *error_message = "TriangleMesh::validate: Triangle3 cell must have exactly 3 nodes.";
            }

            return false;
        }

        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];

        if (!valid_node_id(n0) ||
            !valid_node_id(n1) ||
            !valid_node_id(n2))
        {
            if (error_message)
            {
                *error_message = "TriangleMesh::validate: invalid node id found.";
            }

            return false;
        }

        if (n0 == n1 || n1 == n2 || n2 == n0)
        {
            if (error_message)
            {
                *error_message = "TriangleMesh::validate: duplicated node id in triangle.";
            }

            return false;
        }

        if (area(static_cast<int>(ci)) <= area_eps)
        {
            if (error_message)
            {
                *error_message = "TriangleMesh::validate: degenerated triangle cell found.";
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
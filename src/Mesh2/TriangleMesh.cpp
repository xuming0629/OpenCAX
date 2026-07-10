/**
 * @file TriangleMesh.cpp
 * @brief OpenCAX Mesh2 二维三角形网格类实现
 *
 * TriangleMesh 是 Mesh 的派生类，专门用于二维三角形网格。
 *
 * 当前支持：
 *
 * - 结构矩形区域三角网格生成
 * - 非结构三角网格创建
 * - Triangle3 / Triangle6 面积计算
 * - Triangle3 / Triangle6 质心计算
 * - 三角形网格合法性检查
 *
 * @note
 * TriangleMesh 只负责三角形网格的数据和基础几何计算。
 * 全局拓扑关系，例如 edge2cell、cell2edge、cell2cell、
 * boundary_edges 等，应由 MeshTopology 构建。
 */

#include <OpenCAX/Mesh2/TriangleMesh.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace OpenCAX
{

namespace
{

/**
 * @brief 计算三点在 XY 平面上的二倍有向面积
 *
 * 结果含义：
 *
 * - > 0：a, b, c 为逆时针方向
 * - < 0：a, b, c 为顺时针方向
 * - = 0：三点共线
 *
 * @note
 * 该函数只考虑 XY 平面，主要用于二维三角网格统一单元方向。
 */
static double signed_area2_xy(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    return (b.x - a.x) * (c.y - a.y) -
           (b.y - a.y) * (c.x - a.x);
}

/**
 * @brief 将三角形节点顺序调整为 XY 平面逆时针方向
 *
 * 如果输入三角形为顺时针方向，则交换 b 和 c。
 *
 * @param mesh 所属三角形网格
 * @param a 第一个节点 ID
 * @param b 第二个节点 ID
 * @param c 第三个节点 ID
 *
 * @return 逆时针排列后的三角形节点 ID
 *
 * @note
 * 该函数主要用于 create_unstructured()。
 */
static std::array<int, 3> make_ccw_xy(
    const TriangleMesh& mesh,
    int a,
    int b,
    int c
)
{
    const auto& nodes =
        mesh.nodes();

    if (signed_area2_xy(nodes[a], nodes[b], nodes[c]) < 0.0)
    {
        return {a, c, b};
    }

    return {a, b, c};
}

} // anonymous namespace


/**
 * @brief 构造空三角形网格
 *
 * 初始化网格基本信息：
 *
 * - name      = "TriangleMesh"
 * - source    = "OpenCAX"
 * - dimension = Dim2
 */
TriangleMesh::TriangleMesh()
{
    info_.name = "TriangleMesh";
    info_.source = "OpenCAX";
    info_.dimension = MeshDimension::Dim2;
}


/**
 * @brief 创建矩形区域上的结构三角形网格
 *
 * 将矩形区域 [xmin,xmax] × [ymin,ymax] 划分为 nx × ny
 * 个矩形单元，每个矩形再拆分为两个 Triangle3。
 *
 * 节点编号方式：
 *
 * @code
 * nid(i,j) = j * (nx + 1) + i
 * @endcode
 *
 * 对于一个小矩形：
 *
 * @code
 * n01 ----- n11
 *  |         |
 *  |         |
 * n00 ----- n10
 * @endcode
 *
 * 默认拆分：
 *
 * @code
 * Triangle 1: n00, n10, n11
 * Triangle 2: n11, n01, n00
 * @endcode
 *
 * 若 alternate_diagonal=true，则相邻矩形交替使用另一条对角线。
 *
 * @throws std::invalid_argument 当 nx/ny 非正或边界非法时抛出
 */
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
    if (nx <= 0 || ny <= 0)
    {
        throw std::invalid_argument(
            "TriangleMesh::create_structured_rectangle: nx/ny must be positive."
        );
    }

    if (!(xmax > xmin) || !(ymax > ymin))
    {
        throw std::invalid_argument(
            "TriangleMesh::create_structured_rectangle: invalid bounds."
        );
    }

    TriangleMesh mesh;

    mesh.info_.name = "StructuredTriangleMesh";
    mesh.info_.source = "OpenCAX::Structured";
    mesh.info_.dimension = MeshDimension::Dim2;
    mesh.info_.source_type = MeshSourceType::Structured;
    mesh.info_.structured = true;
    mesh.info_.high_order = false;

    mesh.structured_info_ =
    {
        nx,
        ny,
        xmin,
        xmax,
        ymin,
        ymax,
        alternate_diagonal
    };

    const double dx =
        (xmax - xmin) / static_cast<double>(nx);

    const double dy =
        (ymax - ymin) / static_cast<double>(ny);

    auto nid =
        [nx](int i, int j)
        {
            return j * (nx + 1) + i;
        };

    // =====================================================
    // Step 1:
    // 创建结构网格节点
    // =====================================================
    for (int j = 0; j <= ny; ++j)
    {
        for (int i = 0; i <= nx; ++i)
        {
            mesh.add_node(
                xmin + static_cast<double>(i) * dx,
                ymin + static_cast<double>(j) * dy,
                0.0
            );
        }
    }

    // =====================================================
    // Step 2:
    // 每个矩形拆分为两个三角形
    // =====================================================
    for (int j = 0; j < ny; ++j)
    {
        for (int i = 0; i < nx; ++i)
        {
            const int n00 = nid(i,     j);
            const int n10 = nid(i + 1, j);
            const int n01 = nid(i,     j + 1);
            const int n11 = nid(i + 1, j + 1);

            const bool flip =
                alternate_diagonal &&
                ((i + j) % 2 == 1);

            if (!flip)
            {
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


/**
 * @brief 创建非结构三角形网格
 *
 * 根据外部输入节点坐标和三角形连接关系创建 Triangle3 网格。
 *
 * @param points 节点坐标数组
 * @param triangles 三角形连接关系
 *
 * @throws std::invalid_argument 当 points 或 triangles 为空时抛出
 * @throws std::out_of_range 当三角形节点编号非法时抛出
 * @throws std::invalid_argument 当三角形中存在重复节点时抛出
 *
 * @note
 * 输入三角形会自动调整为 XY 平面逆时针方向。
 */
TriangleMesh TriangleMesh::create_unstructured(
    const std::vector<std::array<double, 3>>& points,
    const std::vector<std::array<int, 3>>& triangles
)
{
    if (points.empty())
    {
        throw std::invalid_argument(
            "TriangleMesh::create_unstructured: points is empty."
        );
    }

    if (triangles.empty())
    {
        throw std::invalid_argument(
            "TriangleMesh::create_unstructured: triangles is empty."
        );
    }

    TriangleMesh mesh;

    mesh.info_.name = "UnstructuredTriangleMesh";
    mesh.info_.source = "OpenCAX::Unstructured";
    mesh.info_.dimension = MeshDimension::Dim2;
    mesh.info_.source_type = MeshSourceType::Unstructured;
    mesh.info_.structured = false;
    mesh.info_.high_order = false;

    // 添加节点
    for (const auto& point : points)
    {
        mesh.add_node(
            point[0],
            point[1],
            point[2]
        );
    }

    // 添加三角形单元
    for (const auto& triangle : triangles)
    {
        if (!mesh.valid_node_id(triangle[0]) ||
            !mesh.valid_node_id(triangle[1]) ||
            !mesh.valid_node_id(triangle[2]))
        {
            throw std::out_of_range(
                "TriangleMesh::create_unstructured: invalid node id."
            );
        }

        if (triangle[0] == triangle[1] ||
            triangle[1] == triangle[2] ||
            triangle[2] == triangle[0])
        {
            throw std::invalid_argument(
                "TriangleMesh::create_unstructured: duplicated node id."
            );
        }

        const auto ccw =
            make_ccw_xy(
                mesh,
                triangle[0],
                triangle[1],
                triangle[2]
            );

        mesh.add_cell(
            CellType::Triangle3,
            {ccw[0], ccw[1], ccw[2]}
        );
    }

    return mesh;
}


/**
 * @brief 设置结构网格参数信息
 *
 * 该函数只修改 structured_info_，不会重新生成节点和单元。
 */
void TriangleMesh::set_structured_info(
    const StructuredGridInfo2D& info
)
{
    structured_info_ = info;
}


/**
 * @brief 获取结构网格参数信息
 */
const StructuredGridInfo2D& TriangleMesh::structured_info() const
{
    return structured_info_;
}


/**
 * @brief 判断是否为结构三角形网格
 */
bool TriangleMesh::is_structured() const
{
    return info_.source_type == MeshSourceType::Structured ||
           info_.structured;
}


/**
 * @brief 判断是否为非结构三角形网格
 *
 * 当前认为以下来源属于非结构网格：
 *
 * - Unstructured
 * - Gmsh
 * - Imported
 */
bool TriangleMesh::is_unstructured() const
{
    return info_.source_type == MeshSourceType::Unstructured ||
           info_.source_type == MeshSourceType::Gmsh ||
           info_.source_type == MeshSourceType::Imported;
}


/**
 * @brief 计算三角形面积
 *
 * 使用三维向量叉积计算面积，因此支持：
 *
 * - XY 平面三角形
 * - 三维空间曲面三角形
 *
 * 面积公式：
 *
 * @code
 * area = 0.5 * | (b - a) × (c - a) |
 * @endcode
 */
double TriangleMesh::triangle_area(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    const double ux = b.x - a.x;
    const double uy = b.y - a.y;
    const double uz = b.z - a.z;

    const double vx = c.x - a.x;
    const double vy = c.y - a.y;
    const double vz = c.z - a.z;

    const double cx = uy * vz - uz * vy;
    const double cy = uz * vx - ux * vz;
    const double cz = ux * vy - uy * vx;

    return 0.5 *
           std::sqrt(
               cx * cx +
               cy * cy +
               cz * cz
           );
}


/**
 * @brief 计算指定三角形单元面积
 *
 * @param cell_id 单元编号
 *
 * @return 单元面积
 *
 * @note
 * 如果 cell_id 非法，返回 0。
 *
 * @note
 * 对 Triangle6，默认使用前三个角点计算面积。
 */
double TriangleMesh::area(
    int cell_id
) const
{
    if (!valid_cell_id(cell_id))
    {
        return 0.0;
    }

    const auto& cell =
        cells_[static_cast<std::size_t>(cell_id)];

    if ((cell.type != CellType::Triangle3 &&
         cell.type != CellType::Triangle6) ||
        cell.node_ids.size() < 3)
    {
        return 0.0;
    }

    return triangle_area(
        nodes_[static_cast<std::size_t>(cell.node_ids[0])],
        nodes_[static_cast<std::size_t>(cell.node_ids[1])],
        nodes_[static_cast<std::size_t>(cell.node_ids[2])]
    );
}


/**
 * @brief 计算网格总面积
 */
double TriangleMesh::total_area() const
{
    double total = 0.0;

    for (std::size_t i = 0; i < cells_.size(); ++i)
    {
        total += area(
            static_cast<int>(i)
        );
    }

    return total;
}


/**
 * @brief 计算指定三角形单元质心
 *
 * @note
 * 对 Triangle6，默认使用前三个角点节点计算几何质心。
 */
std::array<double, 3> TriangleMesh::centroid(
    int cell_id
) const
{
    if (!valid_cell_id(cell_id))
    {
        return {0.0, 0.0, 0.0};
    }

    const auto& cell =
        cells_[static_cast<std::size_t>(cell_id)];

    if ((cell.type != CellType::Triangle3 &&
         cell.type != CellType::Triangle6) ||
        cell.node_ids.size() < 3)
    {
        return {0.0, 0.0, 0.0};
    }

    const auto& a =
        nodes_[static_cast<std::size_t>(cell.node_ids[0])];

    const auto& b =
        nodes_[static_cast<std::size_t>(cell.node_ids[1])];

    const auto& c =
        nodes_[static_cast<std::size_t>(cell.node_ids[2])];

    return {
        (a.x + b.x + c.x) / 3.0,
        (a.y + b.y + c.y) / 3.0,
        (a.z + b.z + c.z) / 3.0
    };
}


/**
 * @brief 三角形网格合法性检查
 *
 * 在 Mesh::validate 的基础上进一步检查：
 *
 * - 所有单元必须是 Triangle3 或 Triangle6
 * - 三角形面积必须大于 area_eps
 *
 * @param error_message 错误信息输出，可为空
 * @param area_eps 面积容差
 *
 * @return 合法返回 true，否则返回 false
 */
bool TriangleMesh::validate(
    std::string* error_message,
    double area_eps
) const
{
    if (!Mesh::validate(error_message))
    {
        return false;
    }

    for (std::size_t i = 0; i < cells_.size(); ++i)
    {
        const auto& cell =
            cells_[i];

        if (cell.type != CellType::Triangle3 &&
            cell.type != CellType::Triangle6)
        {
            if (error_message)
            {
                *error_message =
                    "TriangleMesh::validate: unsupported triangle cell type.";
            }

            return false;
        }

        if (area(static_cast<int>(i)) <= area_eps)
        {
            if (error_message)
            {
                *error_message =
                    "TriangleMesh::validate: non-positive triangle area.";
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


/**
 * @brief 默认三角形网格合法性检查
 *
 * 默认面积容差：
 *
 * @code
 * 1.0e-14
 * @endcode
 */
bool TriangleMesh::validate(
    std::string* error_message
) const
{
    return validate(
        error_message,
        1.0e-14
    );
}

} // namespace OpenCAX
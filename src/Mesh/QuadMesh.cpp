/**
 * @file QuadMesh.cpp
 * @brief OpenCAX Mesh2 二维四边形网格类实现
 *
 * QuadMesh 是 Mesh 的派生类，专门用于二维四边形网格。
 *
 * 当前支持：
 *
 * - 结构矩形区域 Quad4 网格生成
 * - 非结构 Quad4 网格创建
 * - Quad4 / Quad8 / Quad9 面积计算
 * - Quad4 / Quad8 / Quad9 质心计算
 * - 四边形网格合法性检查
 *
 * @note
 * QuadMesh 只负责四边形网格的数据管理和基础几何计算。
 * 全局拓扑关系，例如 edge2cell、cell2edge、cell2cell、
 * boundary_edges 等，应由 MeshTopology 构建。
 */

#include <OpenCAX/Mesh/QuadMesh.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace OpenCAX
{

/**
 * @brief 构造空四边形网格
 *
 * 初始化网格基本信息：
 *
 * - name      = "QuadMesh"
 * - source    = "OpenCAX"
 * - dimension = Dim2
 */
QuadMesh::QuadMesh()
{
    info_.name = "QuadMesh";
    info_.source = "OpenCAX";
    info_.dimension = MeshDimension::Dim2;
}

/**
 * @brief 创建矩形区域上的结构四边形网格
 *
 * 将矩形区域 [xmin,xmax] × [ymin,ymax] 划分为 nx × ny 个 Quad4 单元。
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
 * 单元节点顺序为：
 *
 * @code
 * {n00, n10, n11, n01}
 * @endcode
 *
 * @throws std::invalid_argument 当 nx/ny 非正或边界非法时抛出
 */
QuadMesh QuadMesh::create_structured_rectangle(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    int nx,
    int ny
)
{
    if (nx <= 0 || ny <= 0)
    {
        throw std::invalid_argument(
            "QuadMesh::create_structured_rectangle: nx/ny must be positive."
        );
    }

    if (!(xmax > xmin) || !(ymax > ymin))
    {
        throw std::invalid_argument(
            "QuadMesh::create_structured_rectangle: invalid bounds."
        );
    }

    QuadMesh mesh;

    mesh.info_.name = "StructuredQuadMesh";
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
        false
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
    // 创建 Quad4 单元
    // =====================================================
    for (int j = 0; j < ny; ++j)
    {
        for (int i = 0; i < nx; ++i)
        {
            const int n00 = nid(i,     j);
            const int n10 = nid(i + 1, j);
            const int n01 = nid(i,     j + 1);
            const int n11 = nid(i + 1, j + 1);

            mesh.add_cell(
                CellType::Quad4,
                {n00, n10, n11, n01}
            );
        }
    }

    return mesh;
}

/**
 * @brief 创建非结构四边形网格
 *
 * 根据外部输入节点坐标和四边形连接关系创建 Quad4 网格。
 *
 * @param points 节点坐标数组
 * @param quads 四边形连接关系
 *
 * @throws std::invalid_argument 当 points 或 quads 为空时抛出
 * @throws std::out_of_range 当四边形节点编号非法时抛出
 * @throws std::invalid_argument 当四边形中存在重复节点时抛出
 */
QuadMesh QuadMesh::create_unstructured(
    const std::vector<std::array<double, 3>>& points,
    const std::vector<std::array<int, 4>>& quads
)
{
    if (points.empty())
    {
        throw std::invalid_argument(
            "QuadMesh::create_unstructured: points is empty."
        );
    }

    if (quads.empty())
    {
        throw std::invalid_argument(
            "QuadMesh::create_unstructured: quads is empty."
        );
    }

    QuadMesh mesh;

    mesh.info_.name = "UnstructuredQuadMesh";
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

    // 添加四边形单元
    for (const auto& quad : quads)
    {
        for (int node_id : quad)
        {
            if (!mesh.valid_node_id(node_id))
            {
                throw std::out_of_range(
                    "QuadMesh::create_unstructured: invalid node id."
                );
            }
        }

        if (quad[0] == quad[1] ||
            quad[0] == quad[2] ||
            quad[0] == quad[3] ||
            quad[1] == quad[2] ||
            quad[1] == quad[3] ||
            quad[2] == quad[3])
        {
            throw std::invalid_argument(
                "QuadMesh::create_unstructured: duplicated node id."
            );
        }

        mesh.add_cell(
            CellType::Quad4,
            {quad[0], quad[1], quad[2], quad[3]}
        );
    }

    return mesh;
}

/**
 * @brief 设置二维结构网格参数信息
 *
 * 该函数只修改 structured_info_，不会重新生成节点和单元。
 */
void QuadMesh::set_structured_info(
    const StructuredGridInfo2D& info
)
{
    structured_info_ = info;
}

/**
 * @brief 获取二维结构网格参数信息
 */
const StructuredGridInfo2D& QuadMesh::structured_info() const
{
    return structured_info_;
}

/**
 * @brief 计算三角形面积
 *
 * 使用三维向量叉积计算面积。
 *
 * 因此支持：
 *
 * - XY 平面三角形
 * - 三维空间中的曲面三角形
 *
 * 面积公式：
 *
 * @code
 * area = 0.5 * | (b - a) × (c - a) |
 * @endcode
 */
double QuadMesh::triangle_area(
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
 * @brief 计算四边形面积
 *
 * 默认将四边形按对角线 a-c 拆分为两个三角形：
 *
 * @code
 * quad_area = area(a,b,c) + area(a,c,d)
 * @endcode
 *
 * @note
 * 对非平面四边形，该面积是该拆分方式下的近似面积。
 */
double QuadMesh::quad_area(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c,
    const MeshNode& d
)
{
    return triangle_area(a, b, c) +
           triangle_area(a, c, d);
}

/**
 * @brief 计算指定四边形单元面积
 *
 * @param cell_id 单元编号
 *
 * @return 单元面积
 *
 * @note
 * 如果 cell_id 非法，返回 0。
 *
 * @note
 * 对 Quad8 / Quad9，默认使用前四个角点计算面积。
 */
double QuadMesh::area(
    int cell_id
) const
{
    if (!valid_cell_id(cell_id))
    {
        return 0.0;
    }

    const auto& cell =
        cells_[static_cast<std::size_t>(cell_id)];

    if ((cell.type != CellType::Quad4 &&
         cell.type != CellType::Quad8 &&
         cell.type != CellType::Quad9) ||
        cell.node_ids.size() < 4)
    {
        return 0.0;
    }

    return quad_area(
        nodes_[static_cast<std::size_t>(cell.node_ids[0])],
        nodes_[static_cast<std::size_t>(cell.node_ids[1])],
        nodes_[static_cast<std::size_t>(cell.node_ids[2])],
        nodes_[static_cast<std::size_t>(cell.node_ids[3])]
    );
}

/**
 * @brief 计算四边形网格总面积
 */
double QuadMesh::total_area() const
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
 * @brief 计算指定四边形单元质心
 *
 * @note
 * 对 Quad8 / Quad9，默认使用前四个角点节点计算几何质心。
 */
std::array<double, 3> QuadMesh::centroid(
    int cell_id
) const
{
    if (!valid_cell_id(cell_id))
    {
        return {0.0, 0.0, 0.0};
    }

    const auto& cell =
        cells_[static_cast<std::size_t>(cell_id)];

    if ((cell.type != CellType::Quad4 &&
         cell.type != CellType::Quad8 &&
         cell.type != CellType::Quad9) ||
        cell.node_ids.size() < 4)
    {
        return {0.0, 0.0, 0.0};
    }

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    constexpr int count = 4;

    for (int i = 0; i < count; ++i)
    {
        const auto& point =
            nodes_[static_cast<std::size_t>(cell.node_ids[i])];

        x += point.x;
        y += point.y;
        z += point.z;
    }

    return {
        x / static_cast<double>(count),
        y / static_cast<double>(count),
        z / static_cast<double>(count)
    };
}

/**
 * @brief 四边形网格合法性检查
 *
 * 在 Mesh::validate 的基础上进一步检查：
 *
 * - 所有单元必须是 Quad4 / Quad8 / Quad9
 * - 四边形面积必须大于 area_eps
 *
 * @param error_message 错误信息输出，可为空
 * @param area_eps 面积容差
 *
 * @return 合法返回 true，否则返回 false
 */
bool QuadMesh::validate(
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

        if (cell.type != CellType::Quad4 &&
            cell.type != CellType::Quad8 &&
            cell.type != CellType::Quad9)
        {
            if (error_message)
            {
                *error_message =
                    "QuadMesh::validate: unsupported quad cell type.";
            }

            return false;
        }

        if (area(static_cast<int>(i)) <= area_eps)
        {
            if (error_message)
            {
                *error_message =
                    "QuadMesh::validate: non-positive quad area.";
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
 * @brief 默认四边形网格合法性检查
 *
 * 默认面积容差：
 *
 * @code
 * 1.0e-14
 * @endcode
 */
bool QuadMesh::validate(
    std::string* error_message
) const
{
    return validate(
        error_message,
        1.0e-14
    );
}

} // namespace OpenCAX
/**
 * @file CellTopology.cpp
 * @brief OpenCAX Mesh2 单元局部拓扑工具类实现
 *
 * CellTopology 负责描述单个网格单元的局部拓扑关系。
 *
 * 它不负责构建全局拓扑，例如：
 *
 * - edge2cell
 * - cell2edge
 * - node2cell
 * - cell2cell
 * - boundary_edges
 *
 * 这些全局关系由 MeshTopology 根据 CellTopology 提供的局部拓扑构建。
 *
 *
 * ============================================================
 * 一、CellTopology 的核心职责
 * ============================================================
 *
 * 本文件主要实现：
 *
 * - 单元维度查询
 * - 单元阶次查询
 * - 单元节点数量查询
 * - 单元角点数量查询
 * - 单元边数量查询
 * - 单元面数量查询
 * - 单元族类型查询
 * - 单元局部拓扑边
 * - 单元局部显示边
 * - 单元局部面
 * - 单元局部面类型
 *
 *
 * ============================================================
 * 二、高阶单元拓扑说明
 * ============================================================
 *
 * 对于高阶单元，例如：
 *
 * - Line3
 * - Triangle6
 * - Quad8
 * - Quad9
 * - Tetra10
 * - Hexa20
 * - Hexa27
 *
 * 本类区分两种边：
 *
 *
 * 1. topology_edges
 *
 * 数学拓扑边。
 *
 * 用于：
 *
 * - MeshTopology
 * - FEM 邻接关系
 * - cell2cell
 * - boundary_edges
 * - edge2cell
 *
 * 高阶边中点不参与基础拓扑。
 *
 *
 * 2. visual_edges
 *
 * 可视化边。
 *
 * 用于：
 *
 * - VtkViewer
 * - MeshPlotter
 * - 调试显示
 *
 * 对于高阶单元，visual_edges 会将一条高阶边拆成多段，
 * 例如 Triangle6 的一条边：
 *
 *     v0 ---- m01 ---- v1
 *
 * 会被拆成：
 *
 *     v0 -> m01
 *     m01 -> v1
 *
 *
 * ============================================================
 * 三、Triangle6 示例
 * ============================================================
 *
 * Triangle6 节点顺序：
 *
 *        v2
 *        /\
 *       /  \
 *     m20  m12
 *     /      \
 *   v0--m01--v1
 *
 * node_ids:
 *
 *     0 : v0
 *     1 : v1
 *     2 : v2
 *     3 : m01
 *     4 : m12
 *     5 : m20
 *
 * topology_edges:
 *
 *     v0 - v1
 *     v1 - v2
 *     v2 - v0
 *
 * visual_edges:
 *
 *     v0  - m01
 *     m01 - v1
 *     v1  - m12
 *     m12 - v2
 *     v2  - m20
 *     m20 - v0
 */

#include <OpenCAX/Mesh/CellTopology.h>

namespace OpenCAX
{

// ============================================================
// 内部辅助函数
// ============================================================

/**
 * @brief 构造一条二节点边
 *
 * 这里不做排序。
 *
 * 是否排序由 MeshTopology::canonical_edge 统一处理。
 *
 * @param a 边起点节点 ID
 * @param b 边终点节点 ID
 * @return 二节点边
 */
std::array<int, 2> CellTopology::edge(
    int a,
    int b
)
{
    return {a, b};
}

/**
 * @brief 构造一个局部面
 *
 * @param ids 面上的节点 ID
 * @return 面节点 ID 数组
 */
std::vector<int> CellTopology::face(
    std::initializer_list<int> ids
)
{
    return std::vector<int>(ids);
}


// ============================================================
// 单元基本属性查询
// ============================================================

/**
 * @brief 获取单元拓扑维度
 *
 * 注意：
 *
 * 这里返回的是单元拓扑维度，不是空间坐标维度。
 *
 * 例如：
 *
 * - 三维空间中的三角形曲面单元仍然是 Dim2
 * - 三维空间中的线单元仍然是 Dim1
 */
MeshDimension CellTopology::dimension(
    CellType type
)
{
    switch (type)
    {
    case CellType::Vertex1:
        return MeshDimension::Dim0;

    case CellType::Line2:
    case CellType::Line3:
        return MeshDimension::Dim1;

    case CellType::Triangle3:
    case CellType::Triangle6:
    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9:
        return MeshDimension::Dim2;

    case CellType::Tetra4:
    case CellType::Tetra10:
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
    case CellType::Prism6:
    case CellType::Prism15:
    case CellType::Pyramid5:
    case CellType::Pyramid13:
        return MeshDimension::Dim3;

    default:
        return MeshDimension::Unknown;
    }
}

/**
 * @brief 获取单元阶次
 *
 * 一阶单元：
 *
 * - Line2
 * - Triangle3
 * - Quad4
 * - Tetra4
 * - Hexa8
 *
 * 二阶单元：
 *
 * - Line3
 * - Triangle6
 * - Quad8
 * - Quad9
 * - Tetra10
 * - Hexa20
 * - Hexa27
 */
int CellTopology::order(
    CellType type
)
{
    switch (type)
    {
    case CellType::Line3:
    case CellType::Triangle6:
    case CellType::Quad8:
    case CellType::Quad9:
    case CellType::Tetra10:
    case CellType::Hexa20:
    case CellType::Hexa27:
    case CellType::Prism15:
    case CellType::Pyramid13:
        return 2;

    case CellType::Vertex1:
    case CellType::Line2:
    case CellType::Triangle3:
    case CellType::Quad4:
    case CellType::Tetra4:
    case CellType::Hexa8:
    case CellType::Prism6:
    case CellType::Pyramid5:
        return 1;

    default:
        return 0;
    }
}

/**
 * @brief 获取单元总节点数量
 *
 * 对高阶单元，节点数量包含：
 *
 * - 角点节点
 * - 边中点节点
 * - 面中心节点
 * - 体中心节点
 */
int CellTopology::num_nodes(
    CellType type
)
{
    switch (type)
    {
    case CellType::Vertex1:
        return 1;

    case CellType::Line2:
        return 2;

    case CellType::Line3:
        return 3;

    case CellType::Triangle3:
        return 3;

    case CellType::Triangle6:
        return 6;

    case CellType::Quad4:
        return 4;

    case CellType::Quad8:
        return 8;

    case CellType::Quad9:
        return 9;

    case CellType::Tetra4:
        return 4;

    case CellType::Tetra10:
        return 10;

    case CellType::Hexa8:
        return 8;

    case CellType::Hexa20:
        return 20;

    case CellType::Hexa27:
        return 27;

    case CellType::Prism6:
        return 6;

    case CellType::Prism15:
        return 15;

    case CellType::Pyramid5:
        return 5;

    case CellType::Pyramid13:
        return 13;

    default:
        return 0;
    }
}

/**
 * @brief 获取单元角点数量
 *
 * 角点数量不包含高阶边中点、面中心或体中心。
 *
 * 示例：
 *
 * - Triangle6 -> 3
 * - Quad9     -> 4
 * - Tetra10   -> 4
 * - Hexa27    -> 8
 */
int CellTopology::num_vertices(
    CellType type
)
{
    switch (type)
    {
    case CellType::Vertex1:
        return 1;

    case CellType::Line2:
    case CellType::Line3:
        return 2;

    case CellType::Triangle3:
    case CellType::Triangle6:
        return 3;

    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9:
        return 4;

    case CellType::Tetra4:
    case CellType::Tetra10:
        return 4;

    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        return 8;

    case CellType::Prism6:
    case CellType::Prism15:
        return 6;

    case CellType::Pyramid5:
    case CellType::Pyramid13:
        return 5;

    default:
        return 0;
    }
}

/**
 * @brief 获取单元拓扑边数量
 *
 * 这里返回的是数学拓扑边数量，不是 visual_edges 拆分后的数量。
 *
 * 例如：
 *
 * - Triangle6 仍然返回 3
 * - Quad9     仍然返回 4
 * - Tetra10   仍然返回 6
 */
int CellTopology::num_edges(
    CellType type
)
{
    switch (type)
    {
    case CellType::Line2:
    case CellType::Line3:
        return 1;

    case CellType::Triangle3:
    case CellType::Triangle6:
        return 3;

    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9:
        return 4;

    case CellType::Tetra4:
    case CellType::Tetra10:
        return 6;

    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        return 12;

    case CellType::Prism6:
    case CellType::Prism15:
        return 9;

    case CellType::Pyramid5:
    case CellType::Pyramid13:
        return 8;

    default:
        return 0;
    }
}

/**
 * @brief 获取三维单元的拓扑面数量
 *
 * 对二维面单元返回 0。
 */
int CellTopology::num_faces(
    CellType type
)
{
    switch (type)
    {
    case CellType::Tetra4:
    case CellType::Tetra10:
        return 4;

    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        return 6;

    case CellType::Prism6:
    case CellType::Prism15:
        return 5;

    case CellType::Pyramid5:
    case CellType::Pyramid13:
        return 5;

    default:
        return 0;
    }
}


// ============================================================
// 单元类型判断
// ============================================================

bool CellTopology::is_high_order(
    CellType type
)
{
    return order(type) > 1;
}

bool CellTopology::is_line_cell(
    CellType type
)
{
    return dimension(type) == MeshDimension::Dim1;
}

bool CellTopology::is_surface_cell(
    CellType type
)
{
    return dimension(type) == MeshDimension::Dim2;
}

bool CellTopology::is_volume_cell(
    CellType type
)
{
    return dimension(type) == MeshDimension::Dim3;
}

/**
 * @brief 判断是否为单纯形单元
 *
 * 当前主要包括：
 *
 * - Triangle
 * - Tetrahedron
 *
 * 若后续需要，也可以将 Line 视作 simplex。
 */
bool CellTopology::is_simplex(
    CellType type
)
{
    return family(type) == MeshElementFamily::Triangle ||
           family(type) == MeshElementFamily::Tetrahedron;
}

/**
 * @brief 判断是否为张量积单元
 *
 * 当前包括：
 *
 * - Quad
 * - Hexahedron
 */
bool CellTopology::is_tensor_product(
    CellType type
)
{
    return family(type) == MeshElementFamily::Quadrilateral ||
           family(type) == MeshElementFamily::Hexahedron;
}

/**
 * @brief 获取单元族类型
 */
MeshElementFamily CellTopology::family(
    CellType type
)
{
    switch (type)
    {
    case CellType::Vertex1:
        return MeshElementFamily::Vertex;

    case CellType::Line2:
    case CellType::Line3:
        return MeshElementFamily::Line;

    case CellType::Triangle3:
    case CellType::Triangle6:
        return MeshElementFamily::Triangle;

    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9:
        return MeshElementFamily::Quadrilateral;

    case CellType::Tetra4:
    case CellType::Tetra10:
        return MeshElementFamily::Tetrahedron;

    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        return MeshElementFamily::Hexahedron;

    case CellType::Prism6:
    case CellType::Prism15:
        return MeshElementFamily::Prism;

    case CellType::Pyramid5:
    case CellType::Pyramid13:
        return MeshElementFamily::Pyramid;

    default:
        return MeshElementFamily::Unknown;
    }
}

/**
 * @brief 将 CellType 转成字符串
 *
 * 用于：
 *
 * - 日志
 * - 调试
 * - MeshDebugUtils
 * - 异常信息
 */
const char* CellTopology::to_string(
    CellType type
)
{
    switch (type)
    {
    case CellType::Vertex1:
        return "Vertex1";

    case CellType::Line2:
        return "Line2";

    case CellType::Line3:
        return "Line3";

    case CellType::Triangle3:
        return "Triangle3";

    case CellType::Triangle6:
        return "Triangle6";

    case CellType::Quad4:
        return "Quad4";

    case CellType::Quad8:
        return "Quad8";

    case CellType::Quad9:
        return "Quad9";

    case CellType::Tetra4:
        return "Tetra4";

    case CellType::Tetra10:
        return "Tetra10";

    case CellType::Hexa8:
        return "Hexa8";

    case CellType::Hexa20:
        return "Hexa20";

    case CellType::Hexa27:
        return "Hexa27";

    case CellType::Prism6:
        return "Prism6";

    case CellType::Prism15:
        return "Prism15";

    case CellType::Pyramid5:
        return "Pyramid5";

    case CellType::Pyramid13:
        return "Pyramid13";

    default:
        return "Unknown";
    }
}

/**
 * @brief 获取角点局部索引
 *
 * 目前 OpenCAX 内部约定：
 *
 * - 角点节点总是在 node_ids 前部
 * - 高阶节点在角点之后
 *
 * 因此可以直接返回：
 *
 *     0, 1, ..., num_vertices(type)-1
 */
std::vector<int> CellTopology::vertex_indices(
    CellType type
)
{
    const int n =
        num_vertices(type);

    std::vector<int> ids;

    ids.reserve(
        static_cast<std::size_t>(n)
    );

    for (int i = 0; i < n; ++i)
    {
        ids.push_back(i);
    }

    return ids;
}


// ============================================================
// 数学拓扑边
// ============================================================

/**
 * @brief 获取单元数学拓扑边
 *
 * 该函数用于 MeshTopology 构建全局拓扑。
 *
 * 对于高阶单元，只使用角点节点构造拓扑边。
 */
std::vector<std::array<int, 2>> CellTopology::topology_edges(
    CellType type,
    const std::vector<int>& n
)
{
    switch (type)
    {
    case CellType::Line2:
    case CellType::Line3:
        if (n.size() >= 2)
        {
            return {
                edge(n[0], n[1])
            };
        }
        break;

    case CellType::Triangle3:
    case CellType::Triangle6:
        if (n.size() >= 3)
        {
            return {
                edge(n[0], n[1]),
                edge(n[1], n[2]),
                edge(n[2], n[0])
            };
        }
        break;

    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9:
        if (n.size() >= 4)
        {
            return {
                edge(n[0], n[1]),
                edge(n[1], n[2]),
                edge(n[2], n[3]),
                edge(n[3], n[0])
            };
        }
        break;

    case CellType::Tetra4:
    case CellType::Tetra10:
        if (n.size() >= 4)
        {
            return {
                edge(n[0], n[1]),
                edge(n[1], n[2]),
                edge(n[2], n[0]),

                edge(n[0], n[3]),
                edge(n[1], n[3]),
                edge(n[2], n[3])
            };
        }
        break;

    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        if (n.size() >= 8)
        {
            return {
                edge(n[0], n[1]),
                edge(n[1], n[2]),
                edge(n[2], n[3]),
                edge(n[3], n[0]),

                edge(n[4], n[5]),
                edge(n[5], n[6]),
                edge(n[6], n[7]),
                edge(n[7], n[4]),

                edge(n[0], n[4]),
                edge(n[1], n[5]),
                edge(n[2], n[6]),
                edge(n[3], n[7])
            };
        }
        break;

    case CellType::Prism6:
    case CellType::Prism15:
        if (n.size() >= 6)
        {
            return {
                edge(n[0], n[1]),
                edge(n[1], n[2]),
                edge(n[2], n[0]),

                edge(n[3], n[4]),
                edge(n[4], n[5]),
                edge(n[5], n[3]),

                edge(n[0], n[3]),
                edge(n[1], n[4]),
                edge(n[2], n[5])
            };
        }
        break;

    case CellType::Pyramid5:
    case CellType::Pyramid13:
        if (n.size() >= 5)
        {
            return {
                edge(n[0], n[1]),
                edge(n[1], n[2]),
                edge(n[2], n[3]),
                edge(n[3], n[0]),

                edge(n[0], n[4]),
                edge(n[1], n[4]),
                edge(n[2], n[4]),
                edge(n[3], n[4])
            };
        }
        break;

    default:
        break;
    }

    return {};
}


// ============================================================
// 可视化边
// ============================================================

/**
 * @brief 获取单元可视化边
 *
 * 对于一阶单元，返回 topology_edges。
 *
 * 对于部分二阶单元，会将高阶边拆分成多条线段。
 *
 * 例如 Triangle6:
 *
 *     topology_edges:
 *
 *         v0-v1
 *         v1-v2
 *         v2-v0
 *
 *     visual_edges:
 *
 *         v0-m01
 *         m01-v1
 *         v1-m12
 *         m12-v2
 *         v2-m20
 *         m20-v0
 */
std::vector<std::array<int, 2>> CellTopology::visual_edges(
    CellType type,
    const std::vector<int>& n
)
{
    switch (type)
    {
    case CellType::Line3:
        if (n.size() >= 3)
        {
            return {
                edge(n[0], n[2]),
                edge(n[2], n[1])
            };
        }
        break;

    case CellType::Triangle6:
        if (n.size() >= 6)
        {
            return {
                edge(n[0], n[3]),
                edge(n[3], n[1]),

                edge(n[1], n[4]),
                edge(n[4], n[2]),

                edge(n[2], n[5]),
                edge(n[5], n[0])
            };
        }
        break;

    case CellType::Quad8:
    case CellType::Quad9:
        if (n.size() >= 8)
        {
            return {
                edge(n[0], n[4]),
                edge(n[4], n[1]),

                edge(n[1], n[5]),
                edge(n[5], n[2]),

                edge(n[2], n[6]),
                edge(n[6], n[3]),

                edge(n[3], n[7]),
                edge(n[7], n[0])
            };
        }
        break;

    case CellType::Tetra10:
        if (n.size() >= 10)
        {
            return {
                edge(n[0], n[4]),
                edge(n[4], n[1]),

                edge(n[1], n[5]),
                edge(n[5], n[2]),

                edge(n[2], n[6]),
                edge(n[6], n[0]),

                edge(n[0], n[7]),
                edge(n[7], n[3]),

                edge(n[1], n[8]),
                edge(n[8], n[3]),

                edge(n[2], n[9]),
                edge(n[9], n[3])
            };
        }
        break;

    default:
        break;
    }

    return topology_edges(
        type,
        n
    );
}

/**
 * @brief 向后兼容接口
 *
 * 默认返回 topology_edges。
 *
 * 如果用于显示高阶单元，请使用 visual_edges。
 */
std::vector<std::array<int, 2>> CellTopology::edges(
    CellType type,
    const std::vector<int>& node_ids
)
{
    return topology_edges(
        type,
        node_ids
    );
}


// ============================================================
// 单元局部面
// ============================================================

/**
 * @brief 获取体单元的局部面
 *
 * 对于二维面单元，返回空。
 *
 * 对于高阶体单元，当前返回角点拓扑面。
 *
 * 例如：
 *
 * - Tetra10 返回 4 个 Triangle3 拓扑面
 * - Hexa20 返回 6 个 Quad4 拓扑面
 */
std::vector<std::vector<int>> CellTopology::faces(
    CellType type,
    const std::vector<int>& n
)
{
    switch (type)
    {
    case CellType::Tetra4:
    case CellType::Tetra10:
        if (n.size() >= 4)
        {
            return {
                face({n[0], n[2], n[1]}),
                face({n[0], n[1], n[3]}),
                face({n[1], n[2], n[3]}),
                face({n[2], n[0], n[3]})
            };
        }
        break;

    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        if (n.size() >= 8)
        {
            return {
                face({n[0], n[1], n[2], n[3]}),
                face({n[4], n[5], n[6], n[7]}),

                face({n[0], n[1], n[5], n[4]}),
                face({n[1], n[2], n[6], n[5]}),
                face({n[2], n[3], n[7], n[6]}),
                face({n[3], n[0], n[4], n[7]})
            };
        }
        break;

    case CellType::Prism6:
    case CellType::Prism15:
        if (n.size() >= 6)
        {
            return {
                face({n[0], n[2], n[1]}),
                face({n[3], n[4], n[5]}),

                face({n[0], n[1], n[4], n[3]}),
                face({n[1], n[2], n[5], n[4]}),
                face({n[2], n[0], n[3], n[5]})
            };
        }
        break;

    case CellType::Pyramid5:
    case CellType::Pyramid13:
        if (n.size() >= 5)
        {
            return {
                face({n[0], n[1], n[2], n[3]}),

                face({n[0], n[1], n[4]}),
                face({n[1], n[2], n[4]}),
                face({n[2], n[3], n[4]}),
                face({n[3], n[0], n[4]})
            };
        }
        break;

    default:
        break;
    }

    return {};
}

/**
 * @brief 获取体单元局部面的面类型
 */
std::vector<CellType> CellTopology::face_types(
    CellType type
)
{
    switch (type)
    {
    case CellType::Tetra4:
    case CellType::Tetra10:
        return {
            CellType::Triangle3,
            CellType::Triangle3,
            CellType::Triangle3,
            CellType::Triangle3
        };

    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        return {
            CellType::Quad4,
            CellType::Quad4,
            CellType::Quad4,
            CellType::Quad4,
            CellType::Quad4,
            CellType::Quad4
        };

    case CellType::Prism6:
    case CellType::Prism15:
        return {
            CellType::Triangle3,
            CellType::Triangle3,
            CellType::Quad4,
            CellType::Quad4,
            CellType::Quad4
        };

    case CellType::Pyramid5:
    case CellType::Pyramid13:
        return {
            CellType::Quad4,
            CellType::Triangle3,
            CellType::Triangle3,
            CellType::Triangle3,
            CellType::Triangle3
        };

    default:
        return {};
    }
}

} // namespace OpenCAX
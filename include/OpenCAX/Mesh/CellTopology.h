#pragma once

/**
 * @file CellTopology.h
 * @brief OpenCAX 网格单元局部拓扑工具类声明
 *
 * CellTopology 用于描述不同网格单元类型的“局部拓扑结构”，例如：
 *
 * - 单元维度
 * - 单元阶次
 * - 单元节点数
 * - 单元角点数
 * - 单元边数
 * - 单元面数
 * - 单元局部边连接关系
 * - 单元局部面连接关系
 * - 高阶单元的显示边
 *
 * @note
 * 本类只描述单个单元内部的局部拓扑，不负责构建全局拓扑。
 * 全局 edge、face、cell2cell、boundary 等关系由 MeshTopology 构建。
 *
 * @note
 * 对于高阶单元，例如 Triangle6、Quad8、Tetra10、Hexa20，
 * 本类区分两类边：
 *
 * - topology_edges：数学/拓扑边，用于 MeshTopology、FEM 邻接、边界识别
 * - visual_edges：可视化边，用于绘制高阶单元时经过边中点显示
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/MeshTypes.h>

#include <array>
#include <initializer_list>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 网格单元局部拓扑工具类
 *
 * CellTopology 是一个纯静态工具类，用于根据 CellType 查询单元的局部拓扑信息。
 *
 * 典型用途：
 *
 * @code
 * auto dim = CellTopology::dimension(CellType::Triangle6);
 * auto edges = CellTopology::topology_edges(type, node_ids);
 * auto visual = CellTopology::visual_edges(type, node_ids);
 * @endcode
 */
class OpenCAX_API CellTopology
{
public:
    /**
     * @brief 获取单元的几何维度
     *
     * 示例：
     *
     * - Vertex1    -> Dim0
     * - Line2      -> Dim1
     * - Triangle3  -> Dim2
     * - Quad4      -> Dim2
     * - Tetra4     -> Dim3
     * - Hexa8      -> Dim3
     *
     * @param type 单元类型
     * @return 单元维度
     */
    static MeshDimension dimension(CellType type);

    /**
     * @brief 获取单元阶次
     *
     * 示例：
     *
     * - Line2、Triangle3、Quad4、Tetra4、Hexa8 -> 1
     * - Line3、Triangle6、Quad8、Quad9、Tetra10 -> 2
     *
     * @param type 单元类型
     * @return 单元阶次；未知类型返回 0
     */
    static int order(CellType type);

    /**
     * @brief 获取单元总节点数
     *
     * 注意：
     *
     * - 对一阶单元，总节点数等于角点数
     * - 对高阶单元，总节点数包含边中点、面中心、体中心等高阶节点
     *
     * 示例：
     *
     * - Triangle3 -> 3
     * - Triangle6 -> 6
     * - Quad4     -> 4
     * - Quad9     -> 9
     * - Tetra10   -> 10
     * - Hexa27    -> 27
     *
     * @param type 单元类型
     * @return 单元节点总数；未知类型返回 0
     */
    static int num_nodes(CellType type);

    /**
     * @brief 获取单元角点数量
     *
     * 角点是决定单元拓扑骨架的顶点节点，不包含边中点、面中心或体中心。
     *
     * 示例：
     *
     * - Triangle3 / Triangle6 -> 3
     * - Quad4 / Quad8 / Quad9 -> 4
     * - Tetra4 / Tetra10      -> 4
     * - Hexa8 / Hexa20 / Hexa27 -> 8
     *
     * @param type 单元类型
     * @return 单元角点数；未知类型返回 0
     */
    static int num_vertices(CellType type);

    /**
     * @brief 获取单元拓扑边数量
     *
     * 注意这里返回的是“数学拓扑边”的数量，而不是可视化拆分边数量。
     *
     * 例如：
     *
     * - Triangle6 的拓扑边数量仍然是 3
     * - Quad9 的拓扑边数量仍然是 4
     * - Tetra10 的拓扑边数量仍然是 6
     *
     * @param type 单元类型
     * @return 单元拓扑边数量；未知类型返回 0
     */
    static int num_edges(CellType type);

    /**
     * @brief 获取三维体单元的拓扑面数量
     *
     * 仅对体单元有意义。
     *
     * 示例：
     *
     * - Tetra4 / Tetra10 -> 4
     * - Hexa8 / Hexa20 / Hexa27 -> 6
     * - Prism6 / Prism15 -> 5
     * - Pyramid5 / Pyramid13 -> 5
     *
     * 对二维面单元通常返回 0。
     *
     * @param type 单元类型
     * @return 单元拓扑面数量
     */
    static int num_faces(CellType type);

    /**
     * @brief 判断是否为高阶单元
     *
     * 高阶单元指 order(type) > 1 的单元。
     *
     * @param type 单元类型
     * @return 若为高阶单元返回 true，否则返回 false
     */
    static bool is_high_order(CellType type);

    /**
     * @brief 判断是否为一维线单元
     *
     * @param type 单元类型
     * @return 若单元维度为 Dim1 返回 true
     */
    static bool is_line_cell(CellType type);

    /**
     * @brief 判断是否为二维面单元
     *
     * 包括 Triangle、Quad 等。
     *
     * @param type 单元类型
     * @return 若单元维度为 Dim2 返回 true
     */
    static bool is_surface_cell(CellType type);

    /**
     * @brief 判断是否为三维体单元
     *
     * 包括 Tetra、Hexa、Prism、Pyramid 等。
     *
     * @param type 单元类型
     * @return 若单元维度为 Dim3 返回 true
     */
    static bool is_volume_cell(CellType type);

    /**
     * @brief 判断是否为单纯形单元
     *
     * 单纯形单元包括：
     *
     * - Line
     * - Triangle
     * - Tetrahedron
     *
     * 当前主要用于区分三角形/四面体与四边形/六面体等张量积单元。
     *
     * @param type 单元类型
     * @return 若为单纯形单元返回 true
     */
    static bool is_simplex(CellType type);

    /**
     * @brief 判断是否为张量积型单元
     *
     * 张量积型单元包括：
     *
     * - Quad
     * - Hexahedron
     *
     * 这类单元通常具有规则的局部坐标结构。
     *
     * @param type 单元类型
     * @return 若为张量积型单元返回 true
     */
    static bool is_tensor_product(CellType type);

    /**
     * @brief 获取单元所属的单元族
     *
     * 例如：
     *
     * - Triangle3 / Triangle6 -> Triangle
     * - Quad4 / Quad8 / Quad9 -> Quadrilateral
     * - Tetra4 / Tetra10      -> Tetrahedron
     * - Hexa8 / Hexa20 / Hexa27 -> Hexahedron
     *
     * @param type 单元类型
     * @return 单元族类型
     */
    static MeshElementFamily family(CellType type);

    /**
     * @brief 将 CellType 转换为字符串
     *
     * 用于日志输出、调试打印、错误信息等。
     *
     * 示例：
     *
     * @code
     * CellTopology::to_string(CellType::Triangle6); // "Triangle6"
     * @endcode
     *
     * @param type 单元类型
     * @return 单元类型名称字符串
     */
    static const char* to_string(CellType type);

    /**
     * @brief 获取单元角点在 node_ids 中的局部索引
     *
     * 对于高阶单元，只返回角点节点的局部索引。
     *
     * 示例：
     *
     * - Triangle6 -> {0, 1, 2}
     * - Quad9     -> {0, 1, 2, 3}
     * - Tetra10   -> {0, 1, 2, 3}
     * - Hexa27    -> {0, 1, 2, 3, 4, 5, 6, 7}
     *
     * @param type 单元类型
     * @return 角点局部索引数组
     */
    static std::vector<int> vertex_indices(CellType type);

    /**
     * @brief 获取单元数学拓扑边
     *
     * topology_edges 返回用于数学拓扑和 FEM 邻接的边。
     *
     * 对于高阶单元，拓扑边只连接角点，不经过边中点。
     *
     * 示例：
     *
     * Triangle6 节点顺序：
     *
     * @code
     * [v0, v1, v2, m01, m12, m20]
     * @endcode
     *
     * topology_edges 返回：
     *
     * @code
     * {v0, v1}
     * {v1, v2}
     * {v2, v0}
     * @endcode
     *
     * 该接口主要用于：
     *
     * - MeshTopology 构建全局边
     * - edge2cell
     * - cell2cell
     * - boundary_edges
     * - FEM 自由度邻接
     *
     * @param type 单元类型
     * @param node_ids 单元节点编号
     * @return 单元拓扑边集合
     */
    static std::vector<std::array<int, 2>> topology_edges(
        CellType type,
        const std::vector<int>& node_ids
    );

    /**
     * @brief 获取单元可视化边
     *
     * visual_edges 返回用于绘图显示的边。
     *
     * 对于一阶单元：
     *
     * - visual_edges 与 topology_edges 相同
     *
     * 对于高阶单元：
     *
     * - visual_edges 会经过边中点或高阶边节点拆分边
     *
     * 示例：
     *
     * Triangle6 节点顺序：
     *
     * @code
     * [v0, v1, v2, m01, m12, m20]
     * @endcode
     *
     * visual_edges 返回：
     *
     * @code
     * {v0, m01}
     * {m01, v1}
     * {v1, m12}
     * {m12, v2}
     * {v2, m20}
     * {m20, v0}
     * @endcode
     *
     * 该接口主要用于：
     *
     * - VtkViewer
     * - MeshPlotter
     * - 调试绘图
     * - 高阶单元线框显示
     *
     * @param type 单元类型
     * @param node_ids 单元节点编号
     * @return 单元显示边集合
     */
    static std::vector<std::array<int, 2>> visual_edges(
        CellType type,
        const std::vector<int>& node_ids
    );

    /**
     * @brief 获取单元边连接关系
     *
     * 这是向后兼容接口。
     *
     * @note
     * 当前 edges 等价于 topology_edges。
     * 如果需要绘制高阶单元，请使用 visual_edges。
     *
     * @param type 单元类型
     * @param node_ids 单元节点编号
     * @return 单元拓扑边集合
     */
    static std::vector<std::array<int, 2>> edges(
        CellType type,
        const std::vector<int>& node_ids
    );

    /**
     * @brief 获取三维体单元的局部面
     *
     * 对二维单元通常返回空。
     *
     * 对三维单元返回各局部面的节点编号。
     *
     * 示例：
     *
     * Tetra4 返回 4 个三角形面。
     *
     * Hexa8 返回 6 个四边形面。
     *
     * @note
     * 对于高阶体单元，faces 默认返回角点面。
     * 例如 Tetra10 的每个面仍返回 Triangle3 拓扑面；
     * Hexa20 的每个面仍返回 Quad4 拓扑面。
     *
     * @param type 单元类型
     * @param node_ids 单元节点编号
     * @return 单元局部面集合
     */
    static std::vector<std::vector<int>> faces(
        CellType type,
        const std::vector<int>& node_ids
    );

    /**
     * @brief 获取三维体单元各局部面的面单元类型
     *
     * 示例：
     *
     * - Tetra4 / Tetra10 -> 4 个 Triangle3
     * - Hexa8 / Hexa20 / Hexa27 -> 6 个 Quad4
     * - Prism6 / Prism15 -> 2 个 Triangle3 + 3 个 Quad4
     * - Pyramid5 / Pyramid13 -> 1 个 Quad4 + 4 个 Triangle3
     *
     * @param type 单元类型
     * @return 局部面类型列表
     */
    static std::vector<CellType> face_types(CellType type);

private:
    /**
     * @brief 构造一条局部边
     *
     * @param a 边起点节点编号
     * @param b 边终点节点编号
     * @return 二节点边
     */
    static std::array<int, 2> edge(int a, int b);

    /**
     * @brief 构造一个局部面
     *
     * @param ids 面上的节点编号列表
     * @return 面节点编号数组
     */
    static std::vector<int> face(std::initializer_list<int> ids);
};

} // namespace OpenCAX
#pragma once

/**
 * @file CellTopology.h
 * @brief OpenCAX 网格单元局部拓扑工具类声明
 *
 * 本文件声明 CellTopology 工具类。
 *
 * CellTopology 主要用于查询单元自身的局部拓扑信息，例如：
 *
 * - 单元拓扑维度
 * - 单元阶次
 * - 单元节点数量
 * - 单元边数量
 * - 单元面数量
 * - 局部边连接关系
 * - 局部面连接关系
 * - 局部面类型
 *
 * MeshTopology 会基于 CellTopology 提供的局部拓扑信息，
 * 构建全局网格拓扑关系。
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
 * CellTopology 是一个无状态工具类。
 *
 * 它只描述单个 CellType 的局部拓扑结构，不保存任何网格实例数据。
 *
 * 主要职责：
 *
 * - 查询单元维度
 * - 查询单元阶次
 * - 查询单元节点数量
 * - 查询单元边数量
 * - 查询单元面数量
 * - 返回局部边连接关系
 * - 返回局部面连接关系
 * - 返回局部面类型
 *
 * @note
 * 当前高阶单元在构建全局拓扑时，仅使用角点拓扑。
 * 例如 Tetra10 在拓扑构建时按 Tetra4 的 4 个角点、6 条边、4 个面处理。
 */
class OpenCAX_API CellTopology
{
public:
    /**
     * @brief 获取单元拓扑维度
     *
     * @param type 单元类型
     * @return 单元拓扑维度
     */
    static MeshDimension dimension(
        CellType type
    );

    /**
     * @brief 获取单元阶次
     *
     * @param type 单元类型
     * @return 单元阶次，未知类型返回 0
     */
    static int order(
        CellType type
    );

    /**
     * @brief 获取单元节点数量
     *
     * @param type 单元类型
     * @return 单元节点数量，未知类型返回 0
     */
    static int num_nodes(
        CellType type
    );

    /**
     * @brief 获取单元局部边数量
     *
     * @param type 单元类型
     * @return 单元局部边数量，不含边的类型返回 0
     */
    static int num_edges(
        CellType type
    );

    /**
     * @brief 获取单元局部面数量
     *
     * @param type 单元类型
     * @return 单元局部面数量，非体单元返回 0
     */
    static int num_faces(
        CellType type
    );

    /**
     * @brief 判断单元是否为高阶单元
     *
     * @param type 单元类型
     * @return 如果单元阶次大于 1，返回 true；否则返回 false
     */
    static bool is_high_order(
        CellType type
    );

    /**
     * @brief 判断单元是否为体单元
     *
     * @param type 单元类型
     * @return 如果单元拓扑维度为 Dim3，返回 true；否则返回 false
     */
    static bool is_volume_cell(
        CellType type
    );

    /**
     * @brief 判断单元是否为面单元
     *
     * @param type 单元类型
     * @return 如果单元拓扑维度为 Dim2，返回 true；否则返回 false
     */
    static bool is_surface_cell(
        CellType type
    );

    /**
     * @brief 判断单元是否为线单元
     *
     * @param type 单元类型
     * @return 如果单元拓扑维度为 Dim1，返回 true；否则返回 false
     */
    static bool is_line_cell(
        CellType type
    );

    /**
     * @brief 将单元类型转换为字符串
     *
     * @param type 单元类型
     * @return 单元类型字符串
     */
    static const char* to_string(
        CellType type
    );

    /**
     * @brief 获取单元局部角点边
     *
     * @param type 单元类型
     * @param node_ids 单元节点 ID 列表
     * @return 局部边列表，每条边由两个节点 ID 组成
     *
     * @note
     * 本函数返回的是局部边，不做全局标准化。
     * MeshTopology 会负责将边标准化为 {min(a,b), max(a,b)} 并去重。
     *
     * @note
     * 对于高阶单元，本函数只返回角点边。
     */
    static std::vector<std::array<int, 2>> edges(
        CellType type,
        const std::vector<int>& node_ids
    );

    /**
     * @brief 获取体单元局部角点面
     *
     * @param type 单元类型
     * @param node_ids 单元节点 ID 列表
     * @return 局部面列表，每个面由若干节点 ID 组成
     *
     * @note
     * 本函数返回的是局部面，保留局部节点顺序。
     * MeshTopology 在全局去重时会对面节点 ID 进行排序标准化。
     *
     * @note
     * 对于高阶单元，本函数只返回角点面。
     */
    static std::vector<std::vector<int>> faces(
        CellType type,
        const std::vector<int>& node_ids
    );

    /**
     * @brief 获取体单元局部面类型
     *
     * @param type 单元类型
     * @return 局部面类型列表
     */
    static std::vector<CellType> face_types(
        CellType type
    );

private:
    /**
     * @brief 创建一条局部边
     *
     * @param a 第一个节点 ID
     * @param b 第二个节点 ID
     * @return 由两个节点 ID 构成的边
     */
    static std::array<int, 2> edge(
        int a,
        int b
    );

    /**
     * @brief 创建一个局部面
     *
     * @param ids 面节点 ID 列表
     * @return 由节点 ID 构成的面
     */
    static std::vector<int> face(
        std::initializer_list<int> ids
    );
};

} // namespace OpenCAX
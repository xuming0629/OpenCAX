#pragma once

/**
 * @file MeshTopology.h
 * @brief OpenCAX 网格全局拓扑关系构建器声明
 *
 * MeshTopology 用于从 Mesh 中保存的节点和单元连接关系构建全局拓扑。
 *
 * 支持的拓扑关系包括：
 *
 * - edge2node    : 边到节点
 * - face2node    : 面到节点
 * - cell2edge    : 单元到边
 * - cell2face    : 单元到面
 * - edge2cell    : 边到单元
 * - face2cell    : 面到单元
 * - node2edge    : 节点到边
 * - node2face    : 节点到面
 * - node2cell    : 节点到单元
 * - node2node    : 节点到相邻节点
 * - edge2face    : 边到面
 * - face2edge    : 面到边
 * - cell2cell    : 单元到相邻单元
 *
 * 同时支持提取：
 *
 * - boundary_edges : 边界边
 * - boundary_faces : 边界面
 * - boundary_nodes : 边界节点
 * - boundary_cells : 边界单元
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 网格拓扑关系构建器
 *
 * MeshTopology 根据 Mesh 中的 nodes / cells 自动构建拓扑关系。
 *
 * 当前支持：
 *
 * - 1D 线网格
 * - 2D 三角形网格
 * - 2D 四边形网格
 * - 3D 四面体网格
 * - 3D 六面体网格
 * - 3D 三棱柱网格
 * - 3D 金字塔网格
 *
 * 高阶单元支持：
 *
 * - Line3
 * - Triangle6
 * - Quad8
 * - Quad9
 * - Tetra10
 * - Hexa20
 * - Hexa27
 * - Prism15
 * - Pyramid13
 *
 * @note
 * 高阶单元在全局拓扑构建中默认使用角点拓扑。
 *
 * @note
 * MeshTopology 为了实现边和面的全局去重，会对边和面进行标准化处理。
 * 其中，边标准化为 {min(a,b), max(a,b)}，
 * 面标准化为节点 ID 升序排列。
 *
 * @note
 * 面标准化会丢失方向信息。如果 FEM 计算中需要局部面方向或法向，
 * 应在 FEMElement 或单元局部拓扑中单独处理。
 */
class OpenCAX_API MeshTopology
{
public:
    /**
     * @brief 清空所有拓扑数据
     */
    void clear();

    /**
     * @brief 根据 Mesh 构建拓扑关系
     *
     * @param mesh 输入网格
     */
    void build(
        const Mesh& mesh
    );

    /**
     * @brief 判断拓扑是否已经构建
     *
     * @return 如果已经构建，返回 true；否则返回 false
     */
    bool built() const;

    /**
     * @brief 获取全局边数量
     *
     * @return 全局边数量
     */
    std::size_t num_edges() const;

    /**
     * @brief 获取全局面数量
     *
     * @return 全局面数量
     */
    std::size_t num_faces() const;

    /**
     * @brief 获取边到节点关系
     *
     * 即 edge2node。
     *
     * @return 全局边数组，每条边包含两个节点 ID
     */
    const std::vector<std::array<int, 2>>& edges() const;

    /**
     * @brief 获取面到节点关系
     *
     * 即 face2node。
     *
     * @return 全局面数组，每个面包含若干节点 ID
     */
    const std::vector<std::vector<int>>& faces() const;

    /**
     * @brief 获取全局面类型
     *
     * 每个 face 对应一个 CellType，常见为 Triangle3 或 Quad4。
     *
     * @return 全局面类型数组
     */
    const std::vector<CellType>& face_types() const;

    /**
     * @brief 获取单元到边关系
     *
     * 即 cell2edge。
     *
     * @return 每个单元对应的全局边 ID 列表
     */
    const std::vector<std::vector<int>>& cell2edge() const;

    /**
     * @brief 获取单元到面关系
     *
     * 即 cell2face。
     *
     * @return 每个单元对应的全局面 ID 列表
     */
    const std::vector<std::vector<int>>& cell2face() const;

    /**
     * @brief 获取边到单元关系
     *
     * 即 edge2cell。
     *
     * @return 每条边对应的相邻单元 ID 列表
     */
    const std::vector<std::vector<int>>& edge2cell() const;

    /**
     * @brief 获取面到单元关系
     *
     * 即 face2cell。
     *
     * @return 每个面对应的相邻单元 ID 列表
     */
    const std::vector<std::vector<int>>& face2cell() const;

    /**
     * @brief 获取节点到边关系
     *
     * 即 node2edge。
     *
     * @return 每个节点对应的相邻边 ID 列表
     */
    const std::vector<std::vector<int>>& node2edge() const;

    /**
     * @brief 获取节点到面关系
     *
     * 即 node2face。
     *
     * @return 每个节点对应的相邻面 ID 列表
     */
    const std::vector<std::vector<int>>& node2face() const;

    /**
     * @brief 获取节点到单元关系
     *
     * 即 node2cell。
     *
     * @return 每个节点对应的相邻单元 ID 列表
     */
    const std::vector<std::vector<int>>& node2cell() const;

    /**
     * @brief 获取节点到相邻节点关系
     *
     * 即 node2node。
     *
     * 两个节点如果通过一条边连接，则互为相邻节点。
     *
     * @return 每个节点对应的相邻节点 ID 列表
     */
    const std::vector<std::vector<int>>& node2node() const;

    /**
     * @brief 获取边到面关系
     *
     * 即 edge2face。
     *
     * @return 每条边对应的相邻面 ID 列表
     */
    const std::vector<std::vector<int>>& edge2face() const;

    /**
     * @brief 获取面到边关系
     *
     * 即 face2edge。
     *
     * @return 每个面对应的全局边 ID 列表
     */
    const std::vector<std::vector<int>>& face2edge() const;

    /**
     * @brief 获取单元到相邻单元关系
     *
     * 即 cell2cell。
     *
     * 邻接规则：
     *
     * - 2D 网格：两个单元共享一条边，则互为相邻单元
     * - 3D 网格：两个单元共享一个面，则互为相邻单元
     *
     * @return 每个单元对应的相邻单元 ID 列表
     */
    const std::vector<std::vector<int>>& cell2cell() const;

    /**
     * @brief 获取边界边 ID 列表
     *
     * 边界边判定：
     *
     * - 2D 网格中，如果一条边只属于一个单元，则它是边界边
     * - 3D 网格中，属于边界面的边也视为边界边
     *
     * @return 边界边 ID 列表
     */
    const std::vector<int>& boundary_edges() const;

    /**
     * @brief 获取边界面 ID 列表
     *
     * 对于 3D 体网格，如果一个面只属于一个单元，则该面是边界面。
     *
     * @return 边界面 ID 列表
     */
    const std::vector<int>& boundary_faces() const;

    /**
     * @brief 获取边界节点 ID 列表
     *
     * @return 边界节点 ID 列表
     */
    const std::vector<int>& boundary_nodes() const;

    /**
     * @brief 获取边界单元 ID 列表
     *
     * 边界单元判定：
     *
     * - 2D 网格中，包含至少一条边界边的单元是边界单元
     * - 3D 网格中，包含至少一个边界面的单元是边界单元
     *
     * @return 边界单元 ID 列表
     */
    const std::vector<int>& boundary_cells() const;

private:
    /**
     * @brief 标准化边
     *
     * 将边统一为 {min(a,b), max(a,b)}，方便全局去重。
     *
     * @param a 第一个节点 ID
     * @param b 第二个节点 ID
     * @return 标准化后的边
     */
    static std::array<int, 2> canonical_edge(
        int a,
        int b
    );

    /**
     * @brief 标准化面
     *
     * 将面节点 ID 按升序排列，方便全局去重。
     *
     * @param node_ids 输入面节点 ID 列表
     * @return 标准化后的面节点 ID 列表
     *
     * @note
     * 该操作会丢失面的方向信息。
     */
    static std::vector<int> canonical_face(
        const std::vector<int>& node_ids
    );

    /**
     * @brief 对数组排序并去重
     *
     * @param values 需要排序去重的整数数组
     */
    static void sort_unique(
        std::vector<int>& values
    );

private:
    /**
     * @brief 是否已经构建拓扑
     */
    bool built_ = false;

    /**
     * @brief 全局边到节点关系
     *
     * 即 edge2node。
     */
    std::vector<std::array<int, 2>> edges_;

    /**
     * @brief 全局面到节点关系
     *
     * 即 face2node。
     */
    std::vector<std::vector<int>> faces_;

    /**
     * @brief 全局面类型数组
     */
    std::vector<CellType> face_types_;

    /**
     * @brief 单元到边关系
     *
     * 即 cell2edge。
     */
    std::vector<std::vector<int>> cell2edge_;

    /**
     * @brief 单元到面关系
     *
     * 即 cell2face。
     */
    std::vector<std::vector<int>> cell2face_;

    /**
     * @brief 边到单元关系
     *
     * 即 edge2cell。
     */
    std::vector<std::vector<int>> edge2cell_;

    /**
     * @brief 面到单元关系
     *
     * 即 face2cell。
     */
    std::vector<std::vector<int>> face2cell_;

    /**
     * @brief 节点到边关系
     *
     * 即 node2edge。
     */
    std::vector<std::vector<int>> node2edge_;

    /**
     * @brief 节点到面关系
     *
     * 即 node2face。
     */
    std::vector<std::vector<int>> node2face_;

    /**
     * @brief 节点到单元关系
     *
     * 即 node2cell。
     */
    std::vector<std::vector<int>> node2cell_;

    /**
     * @brief 节点到相邻节点关系
     *
     * 即 node2node。
     */
    std::vector<std::vector<int>> node2node_;

    /**
     * @brief 边到面关系
     *
     * 即 edge2face。
     */
    std::vector<std::vector<int>> edge2face_;

    /**
     * @brief 面到边关系
     *
     * 即 face2edge。
     */
    std::vector<std::vector<int>> face2edge_;

    /**
     * @brief 单元到相邻单元关系
     *
     * 即 cell2cell。
     */
    std::vector<std::vector<int>> cell2cell_;

    /**
     * @brief 边界边 ID 列表
     */
    std::vector<int> boundary_edges_;

    /**
     * @brief 边界面 ID 列表
     */
    std::vector<int> boundary_faces_;

    /**
     * @brief 边界节点 ID 列表
     */
    std::vector<int> boundary_nodes_;

    /**
     * @brief 边界单元 ID 列表
     */
    std::vector<int> boundary_cells_;
};

} // namespace OpenCAX
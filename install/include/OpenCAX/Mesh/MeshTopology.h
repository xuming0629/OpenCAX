#pragma once

/**
 * @file MeshTopology.h
 * @brief OpenCAX 网格全局拓扑关系构建器声明
 *
 * MeshTopology 根据 Mesh 中的 nodes / cells 自动构建全局拓扑关系。
 *
 * 支持：
 *
 * - edge2node : edges()
 * - face2node : faces()
 * - cell2edge
 * - cell2face
 * - edge2cell
 * - face2cell
 * - node2edge
 * - node2face
 * - node2cell
 * - node2node
 * - edge2face
 * - face2edge
 * - cell2cell
 * - boundary_edges
 * - boundary_faces
 * - boundary_nodes
 * - boundary_cells
 *
 * 设计说明：
 *
 * 1. 对 2D 面网格，边界通过 edge2cell 判断。
 *    如果一条边只属于一个单元，则该边是边界边。
 *
 * 2. 对 3D 体网格，边界通过 face2cell 判断。
 *    如果一个面只属于一个单元，则该面是边界面。
 *
 * 3. edges_ 保存规范化边 {min(a,b), max(a,b)}。
 *
 * 4. faces_ 保存首次出现的局部面节点顺序。
 *    face_map 使用 canonical_face 作为去重 key。
 *    这样既可以正确去重，又不会破坏面的环向顺序。
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 网格拓扑关系构建器
 */
class OpenCAX_API MeshTopology
{
public:
    /**
     * @brief 清空所有拓扑数据
     */
    void clear();

    /**
     * @brief 根据 Mesh 构建拓扑
     *
     * @param mesh 输入网格
     */
    void build(
        const Mesh& mesh
    );

    /**
     * @brief 是否已经构建
     *
     * @return 已经构建返回 true，否则返回 false
     */
    bool built() const;

    /**
     * @brief 边数量
     *
     * @return 全局边数量
     */
    std::size_t num_edges() const;

    /**
     * @brief 面数量
     *
     * @return 全局面数量
     */
    std::size_t num_faces() const;

    /**
     * @brief 获取全局边到节点关系 edge2node
     *
     * 每条边保存两个节点 ID。
     *
     * @return 全局边数组
     */
    const std::vector<std::array<int, 2>>& edges() const;

    /**
     * @brief 获取全局面到节点关系 face2node
     *
     * 每个面保存若干节点 ID。
     *
     * @return 全局面数组
     */
    const std::vector<std::vector<int>>& faces() const;

    /**
     * @brief 获取全局面类型
     *
     * 与 faces() 一一对应。
     *
     * @return 面类型数组
     */
    const std::vector<CellType>& face_types() const;

    /**
     * @brief 获取单元到边关系
     *
     * @return cell2edge 关系
     */
    const std::vector<std::vector<int>>& cell2edge() const;

    /**
     * @brief 获取单元到面关系
     *
     * 对 2D 面网格通常为空。
     * 对 3D 体网格有效。
     *
     * @return cell2face 关系
     */
    const std::vector<std::vector<int>>& cell2face() const;

    /**
     * @brief 获取边到单元关系
     *
     * @return edge2cell 关系
     */
    const std::vector<std::vector<int>>& edge2cell() const;

    /**
     * @brief 获取面到单元关系
     *
     * @return face2cell 关系
     */
    const std::vector<std::vector<int>>& face2cell() const;

    /**
     * @brief 获取节点到边关系
     *
     * @return node2edge 关系
     */
    const std::vector<std::vector<int>>& node2edge() const;

    /**
     * @brief 获取节点到面关系
     *
     * @return node2face 关系
     */
    const std::vector<std::vector<int>>& node2face() const;

    /**
     * @brief 获取节点到单元关系
     *
     * @return node2cell 关系
     */
    const std::vector<std::vector<int>>& node2cell() const;

    /**
     * @brief 获取节点到相邻节点关系
     *
     * 两个节点共享一条边，则互为相邻节点。
     *
     * @return node2node 关系
     */
    const std::vector<std::vector<int>>& node2node() const;

    /**
     * @brief 获取边到面关系
     *
     * 对 3D 体网格有效。
     *
     * @return edge2face 关系
     */
    const std::vector<std::vector<int>>& edge2face() const;

    /**
     * @brief 获取面到边关系
     *
     * 对 3D 体网格有效。
     *
     * @return face2edge 关系
     */
    const std::vector<std::vector<int>>& face2edge() const;

    /**
     * @brief 获取单元到相邻单元关系
     *
     * 对 2D 网格：
     *
     * - 两个单元共享一条边，则互为相邻单元。
     *
     * 对 3D 网格：
     *
     * - 两个单元共享一个面，则互为相邻单元。
     *
     * @return cell2cell 关系
     */
    const std::vector<std::vector<int>>& cell2cell() const;

    /**
     * @brief 获取边界边 ID
     *
     * 对 2D 网格：
     *
     * - edge2cell.size() == 1 的边为边界边。
     *
     * 对 3D 网格：
     *
     * - 边界面上的边为边界边。
     *
     * @return 边界边 ID 数组
     */
    const std::vector<int>& boundary_edges() const;

    /**
     * @brief 获取边界面 ID
     *
     * 对 3D 体网格有效。
     *
     * @return 边界面 ID 数组
     */
    const std::vector<int>& boundary_faces() const;

    /**
     * @brief 获取边界节点 ID
     *
     * @return 边界节点 ID 数组
     */
    const std::vector<int>& boundary_nodes() const;

    /**
     * @brief 获取边界单元 ID
     *
     * 对 2D 网格：
     *
     * - 至少包含一条边界边的单元为边界单元。
     *
     * 对 3D 网格：
     *
     * - 至少包含一个边界面的单元为边界单元。
     *
     * @return 边界单元 ID 数组
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
     * @return 标准化边
     */
    static std::array<int, 2> canonical_edge(
        int a,
        int b
    );

    /**
     * @brief 标准化面
     *
     * 将面的节点 ID 排序，作为全局去重 key。
     *
     * @param node_ids 面节点 ID
     * @return 标准化面 key
     *
     * @note
     * canonical_face 只用于 face_map 去重，不用于 faces_ 存储。
     * faces_ 应保存首次出现时的局部面节点顺序，方便可视化和面边关系构建。
     */
    static std::vector<int> canonical_face(
        const std::vector<int>& node_ids
    );

    /**
     * @brief 对整数数组排序并去重
     *
     * @param values 输入输出整数数组
     */
    static void sort_unique(
        std::vector<int>& values
    );

private:
    /**
     * @brief 是否已经构建
     */
    bool built_ = false;

    /**
     * @brief 全局边到节点关系 edge2node
     */
    std::vector<std::array<int, 2>> edges_;

    /**
     * @brief 全局面到节点关系 face2node
     */
    std::vector<std::vector<int>> faces_;

    /**
     * @brief 全局面类型
     */
    std::vector<CellType> face_types_;

    /**
     * @brief 单元到边关系
     */
    std::vector<std::vector<int>> cell2edge_;

    /**
     * @brief 单元到面关系
     */
    std::vector<std::vector<int>> cell2face_;

    /**
     * @brief 边到单元关系
     */
    std::vector<std::vector<int>> edge2cell_;

    /**
     * @brief 面到单元关系
     */
    std::vector<std::vector<int>> face2cell_;

    /**
     * @brief 节点到边关系
     */
    std::vector<std::vector<int>> node2edge_;

    /**
     * @brief 节点到面关系
     */
    std::vector<std::vector<int>> node2face_;

    /**
     * @brief 节点到单元关系
     */
    std::vector<std::vector<int>> node2cell_;

    /**
     * @brief 节点到相邻节点关系
     */
    std::vector<std::vector<int>> node2node_;

    /**
     * @brief 边到面关系
     */
    std::vector<std::vector<int>> edge2face_;

    /**
     * @brief 面到边关系
     */
    std::vector<std::vector<int>> face2edge_;

    /**
     * @brief 单元到相邻单元关系
     */
    std::vector<std::vector<int>> cell2cell_;

    /**
     * @brief 边界边 ID
     */
    std::vector<int> boundary_edges_;

    /**
     * @brief 边界面 ID
     */
    std::vector<int> boundary_faces_;

    /**
     * @brief 边界节点 ID
     */
    std::vector<int> boundary_nodes_;

    /**
     * @brief 边界单元 ID
     */
    std::vector<int> boundary_cells_;
};

} // namespace OpenCAX
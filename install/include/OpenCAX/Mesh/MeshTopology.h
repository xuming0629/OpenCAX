#pragma once

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
 * - edge2node : edges()
 * - cell2edge
 * - edge2cell
 * - node2edge
 * - node2cell
 * - node2node
 * - cell2cell
 * - boundary_edges
 * - boundary_nodes
 * - boundary_cells
 *
 * 后续三维面拓扑可继续扩展：
 *
 * - faces
 * - cell2face
 * - face2cell
 * - node2face
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
    void build(const Mesh& mesh);

    /**
     * @brief 边到节点关系，即 edge2node
     */
    const std::vector<std::array<int, 2>>& edges() const;

    /**
     * @brief 单元到边关系
     */
    const std::vector<std::vector<int>>& cell2edge() const;

    /**
     * @brief 边到单元关系
     */
    const std::vector<std::vector<int>>& edge2cell() const;

    /**
     * @brief 节点到边关系
     */
    const std::vector<std::vector<int>>& node2edge() const;

    /**
     * @brief 节点到单元关系
     */
    const std::vector<std::vector<int>>& node2cell() const;

    /**
     * @brief 节点到相邻节点关系
     */
    const std::vector<std::vector<int>>& node2node() const;

    /**
     * @brief 单元到相邻单元关系
     *
     * 两个单元共享一条边，则互为相邻单元。
     */
    const std::vector<std::vector<int>>& cell2cell() const;

    /**
     * @brief 边界边 ID
     */
    const std::vector<int>& boundary_edges() const;

    /**
     * @brief 边界节点 ID
     */
    const std::vector<int>& boundary_nodes() const;

    /**
     * @brief 边界单元 ID
     *
     * 至少包含一条边界边的单元。
     */
    const std::vector<int>& boundary_cells() const;

    /**
     * @brief 是否已经构建
     */
    bool built() const;

    /**
     * @brief 边数量
     */
    std::size_t num_edges() const;

private:
    /**
     * @brief 标准化边
     *
     * 将边统一为 {min(a,b), max(a,b)}，方便去重。
     */
    static std::array<int, 2> canonical_edge(
        int a,
        int b
    );

    /**
     * @brief 获取一个单元的局部边
     */
    std::vector<std::array<int, 2>> local_edges(
        CellType type,
        const std::vector<int>& node_ids
    ) const;

private:
    bool built_ = false;

    /**
     * @brief 边到节点关系 edge2node
     */
    std::vector<std::array<int, 2>> edges_;

    /**
     * @brief 单元到边
     */
    std::vector<std::vector<int>> cell2edge_;

    /**
     * @brief 边到单元
     */
    std::vector<std::vector<int>> edge2cell_;

    /**
     * @brief 节点到边
     */
    std::vector<std::vector<int>> node2edge_;

    /**
     * @brief 节点到单元
     */
    std::vector<std::vector<int>> node2cell_;

    /**
     * @brief 节点到节点
     */
    std::vector<std::vector<int>> node2node_;

    /**
     * @brief 单元到相邻单元
     */
    std::vector<std::vector<int>> cell2cell_;

    /**
     * @brief 边界边
     */
    std::vector<int> boundary_edges_;

    /**
     * @brief 边界节点
     */
    std::vector<int> boundary_nodes_;

    /**
     * @brief 边界单元
     */
    std::vector<int> boundary_cells_;
};

} // namespace OpenCAX
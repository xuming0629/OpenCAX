#pragma once

/**
 * @file MeshTopology.h
 * @brief OpenCAX Mesh2 鲁棒全局拓扑关系管理类
 *
 * 支持一阶和二阶：
 * - Triangle3 / Triangle6
 * - Quad4 / Quad8 / Quad9
 * - Tetra4 / Tetra10
 * - Hexa8 / Hexa20 / Hexa27
 *
 * 关键设计：
 * - edges/faces 保存角点拓扑
 * - edge_nodes/face_nodes 保存完整高阶节点
 * - cell2edge/cell2face 保留局部编号顺序，不排序
 * - boundary_nodes 包含所有高阶边界节点
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/Mesh.h>

#include <array>
#include <cstddef>
#include <vector>

namespace OpenCAX
{

enum class TopologyLevel
{
    None  = 0,
    Edges = 1,
    Faces = 2,
    Full  = 3
};

class OpenCAX_API MeshTopology
{
public:
    void clear();

    void build(
        const Mesh& mesh,
        TopologyLevel level = TopologyLevel::Full
    );

    bool built() const;
    TopologyLevel level() const;

    std::size_t num_nodes() const;
    std::size_t num_cells() const;
    std::size_t num_edges() const;
    std::size_t num_faces() const;

    const std::vector<std::array<int, 2>>& edges() const;
    const std::vector<std::vector<int>>& edge_nodes() const;

    const std::vector<std::vector<int>>& faces() const;
    const std::vector<std::vector<int>>& face_nodes() const;
    const std::vector<CellType>& face_types() const;

    const std::vector<std::vector<int>>& cell2edge() const;
    const std::vector<std::vector<int>>& cell2edge_orientation() const;

    const std::vector<std::vector<int>>& cell2face() const;
    const std::vector<std::vector<int>>& cell2face_orientation() const;

    const std::vector<std::vector<int>>& edge2cell() const;
    const std::vector<std::vector<int>>& face2cell() const;

    const std::vector<std::vector<int>>& node2edge() const;
    const std::vector<std::vector<int>>& node2face() const;
    const std::vector<std::vector<int>>& node2cell() const;
    const std::vector<std::vector<int>>& node2node() const;

    const std::vector<std::vector<int>>& edge2face() const;
    const std::vector<std::vector<int>>& face2edge() const;
    const std::vector<std::vector<int>>& cell2cell() const;

    const std::vector<int>& boundary_edges() const;
    const std::vector<int>& boundary_faces() const;
    const std::vector<int>& boundary_nodes() const;
    const std::vector<int>& boundary_cells() const;

    const std::vector<int>& non_manifold_edges() const;
    const std::vector<int>& non_manifold_faces() const;

private:
    static std::array<int, 2> canonical_edge(int a, int b);
    static std::vector<int> canonical_face(const std::vector<int>& node_ids);
    static void sort_unique(std::vector<int>& values);

    static std::vector<int> normalize_edge_nodes(
        const std::vector<int>& local_edge_nodes,
        const std::array<int, 2>& canonical
    );

    static int edge_orientation(
        const std::array<int, 2>& local,
        const std::array<int, 2>& global
    );

    static int face_orientation(
        const std::vector<int>& local,
        const std::vector<int>& global
    );

private:
    bool built_ = false;
    TopologyLevel level_ = TopologyLevel::None;

    std::size_t num_nodes_ = 0;
    std::size_t num_cells_ = 0;

    std::vector<std::array<int, 2>> edges_;
    std::vector<std::vector<int>> edge_nodes_;

    std::vector<std::vector<int>> faces_;
    std::vector<std::vector<int>> face_nodes_;
    std::vector<CellType> face_types_;

    std::vector<std::vector<int>> cell2edge_;
    std::vector<std::vector<int>> cell2edge_orientation_;
    std::vector<std::vector<int>> cell2face_;
    std::vector<std::vector<int>> cell2face_orientation_;

    std::vector<std::vector<int>> edge2cell_;
    std::vector<std::vector<int>> face2cell_;

    std::vector<std::vector<int>> node2edge_;
    std::vector<std::vector<int>> node2face_;
    std::vector<std::vector<int>> node2cell_;
    std::vector<std::vector<int>> node2node_;

    std::vector<std::vector<int>> edge2face_;
    std::vector<std::vector<int>> face2edge_;
    std::vector<std::vector<int>> cell2cell_;

    std::vector<int> boundary_edges_;
    std::vector<int> boundary_faces_;
    std::vector<int> boundary_nodes_;
    std::vector<int> boundary_cells_;

    std::vector<int> non_manifold_edges_;
    std::vector<int> non_manifold_faces_;
};

} // namespace OpenCAX

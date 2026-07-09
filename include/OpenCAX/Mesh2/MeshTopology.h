#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/Mesh.h>

#include <array>
#include <cstddef>
#include <vector>

namespace OpenCAX
{

enum class TopologyLevel
{
    None = 0,
    Edges = 1,
    Faces = 2,
    Full = 3
};

class OpenCAX_API MeshTopology
{
public:
    void clear();
    void build(const Mesh& mesh, TopologyLevel level = TopologyLevel::Full);
    bool built() const;

    std::size_t num_edges() const;
    std::size_t num_faces() const;

    const std::vector<std::array<int, 2>>& edges() const;
    const std::vector<std::vector<int>>& faces() const;
    const std::vector<CellType>& face_types() const;

    const std::vector<std::vector<int>>& cell2edge() const;
    const std::vector<std::vector<int>>& cell2face() const;
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

private:
    static std::array<int, 2> canonical_edge(int a, int b);
    static std::vector<int> canonical_face(const std::vector<int>& node_ids);
    static void sort_unique(std::vector<int>& values);

private:
    bool built_ = false;
    TopologyLevel level_ = TopologyLevel::None;

    std::vector<std::array<int, 2>> edges_;
    std::vector<std::vector<int>> faces_;
    std::vector<CellType> face_types_;

    std::vector<std::vector<int>> cell2edge_;
    std::vector<std::vector<int>> cell2face_;
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
};

} // namespace OpenCAX

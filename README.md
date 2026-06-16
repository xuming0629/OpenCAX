# OpenCAX
# 基于 OpenCasecade 的 CAX 读写工具


cmake -S . -B build \
  -DOpenCASCADE_DIR=/home/xuming/software/occt-7.9.2/lib/cmake/opencascade \
  -DVTK_DIR=/home/xuming/software/vtk-9.4.2/lib/cmake/vtk-9.4 \
  -DGMSH_ROOT=/home/xuming/software/gmsh-4.15

cmake --build build -j8





#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API MeshTopology
{
public:
    void clear();
    void build(const Mesh& mesh);

    const std::vector<std::array<int, 2>>& edges() const;

    const std::vector<std::vector<int>>& cell2edge() const;
    const std::vector<std::vector<int>>& edge2cell() const;

    const std::vector<std::vector<int>>& node2edge() const;
    const std::vector<std::vector<int>>& node2cell() const;
    const std::vector<std::vector<int>>& node2node() const;

    const std::vector<std::vector<int>>& cell2cell() const;

    const std::vector<int>& boundary_edges() const;
    const std::vector<int>& boundary_nodes() const;
    const std::vector<int>& boundary_cells() const;

    bool built() const;

    std::size_t num_edges() const;

private:
    static std::array<int, 2> canonical_edge(
        int a,
        int b
    );

    std::vector<std::array<int, 2>> local_edges(
        CellType type,
        const std::vector<int>& node_ids
    ) const;

private:
    bool built_ = false;

    std::vector<std::array<int, 2>> edges_;

    std::vector<std::vector<int>> cell2edge_;
    std::vector<std::vector<int>> edge2cell_;

    std::vector<std::vector<int>> node2edge_;
    std::vector<std::vector<int>> node2cell_;
    std::vector<std::vector<int>> node2node_;

    std::vector<std::vector<int>> cell2cell_;

    std::vector<int> boundary_edges_;
    std::vector<int> boundary_nodes_;
    std::vector<int> boundary_cells_;
};

} // namespace OpenCAX


#include <OpenCAX/Mesh/MeshTopology.h>

#include <map>
#include <set>
#include <stdexcept>

namespace OpenCAX
{

void MeshTopology::clear()
{
    built_ = false;

    edges_.clear();

    cell2edge_.clear();
    edge2cell_.clear();

    node2edge_.clear();
    node2cell_.clear();
    node2node_.clear();

    cell2cell_.clear();

    boundary_edges_.clear();
    boundary_nodes_.clear();
    boundary_cells_.clear();
}

void MeshTopology::build(const Mesh& mesh)
{
    clear();

    const auto& nodes = mesh.nodes();
    const auto& cells = mesh.cells();

    const int num_nodes = static_cast<int>(nodes.size());
    const int num_cells = static_cast<int>(cells.size());

    if (num_nodes <= 0) {
        throw std::runtime_error(
            "MeshTopology::build failed: mesh has no nodes."
        );
    }

    node2edge_.resize(num_nodes);
    node2cell_.resize(num_nodes);
    node2node_.resize(num_nodes);

    cell2edge_.resize(num_cells);
    cell2cell_.resize(num_cells);

    std::map<std::array<int, 2>, int> edge_map;
    std::vector<std::set<int>> node2edge_set(num_nodes);
    std::vector<std::set<int>> node2node_set(num_nodes);
    std::vector<std::set<int>> cell2cell_set(num_cells);

    for (int cid = 0; cid < num_cells; ++cid) {
        const MeshCell& cell = cells[cid];

        for (int nid : cell.node_ids) {
            if (nid < 0 || nid >= num_nodes) {
                throw std::runtime_error(
                    "MeshTopology::build failed: invalid node id."
                );
            }

            node2cell_[nid].push_back(cid);
        }

        const auto local = local_edges(
            cell.type,
            cell.node_ids
        );

        for (const auto& e : local) {
            const auto ce = canonical_edge(e[0], e[1]);

            auto it = edge_map.find(ce);

            int eid = -1;

            if (it == edge_map.end()) {
                eid = static_cast<int>(edges_.size());

                edge_map[ce] = eid;
                edges_.push_back(ce);
                edge2cell_.push_back({});
            } else {
                eid = it->second;
            }

            cell2edge_[cid].push_back(eid);
            edge2cell_[eid].push_back(cid);

            node2edge_set[ce[0]].insert(eid);
            node2edge_set[ce[1]].insert(eid);

            node2node_set[ce[0]].insert(ce[1]);
            node2node_set[ce[1]].insert(ce[0]);
        }
    }

    for (int nid = 0; nid < num_nodes; ++nid) {
        node2edge_[nid].assign(
            node2edge_set[nid].begin(),
            node2edge_set[nid].end()
        );

        node2node_[nid].assign(
            node2node_set[nid].begin(),
            node2node_set[nid].end()
        );
    }

    std::set<int> boundary_node_set;
    std::set<int> boundary_cell_set;

    for (int eid = 0; eid < static_cast<int>(edge2cell_.size()); ++eid) {
        const auto& adj_cells = edge2cell_[eid];

        if (adj_cells.size() == 1) {
            boundary_edges_.push_back(eid);

            boundary_node_set.insert(edges_[eid][0]);
            boundary_node_set.insert(edges_[eid][1]);

            boundary_cell_set.insert(adj_cells[0]);
        }

        for (std::size_t i = 0; i < adj_cells.size(); ++i) {
            for (std::size_t j = 0; j < adj_cells.size(); ++j) {
                if (i == j) {
                    continue;
                }

                cell2cell_set[adj_cells[i]].insert(adj_cells[j]);
            }
        }
    }

    for (int cid = 0; cid < num_cells; ++cid) {
        cell2cell_[cid].assign(
            cell2cell_set[cid].begin(),
            cell2cell_set[cid].end()
        );
    }

    boundary_nodes_.assign(
        boundary_node_set.begin(),
        boundary_node_set.end()
    );

    boundary_cells_.assign(
        boundary_cell_set.begin(),
        boundary_cell_set.end()
    );

    built_ = true;
}

const std::vector<std::array<int, 2>>& MeshTopology::edges() const
{
    return edges_;
}

const std::vector<std::vector<int>>& MeshTopology::cell2edge() const
{
    return cell2edge_;
}

const std::vector<std::vector<int>>& MeshTopology::edge2cell() const
{
    return edge2cell_;
}

const std::vector<std::vector<int>>& MeshTopology::node2edge() const
{
    return node2edge_;
}

const std::vector<std::vector<int>>& MeshTopology::node2cell() const
{
    return node2cell_;
}

const std::vector<std::vector<int>>& MeshTopology::node2node() const
{
    return node2node_;
}

const std::vector<std::vector<int>>& MeshTopology::cell2cell() const
{
    return cell2cell_;
}

const std::vector<int>& MeshTopology::boundary_edges() const
{
    return boundary_edges_;
}

const std::vector<int>& MeshTopology::boundary_nodes() const
{
    return boundary_nodes_;
}

const std::vector<int>& MeshTopology::boundary_cells() const
{
    return boundary_cells_;
}

bool MeshTopology::built() const
{
    return built_;
}

std::size_t MeshTopology::num_edges() const
{
    return edges_.size();
}

std::array<int, 2> MeshTopology::canonical_edge(
    int a,
    int b
)
{
    if (a < b) {
        return {a, b};
    }

    return {b, a};
}

std::vector<std::array<int, 2>> MeshTopology::local_edges(
    CellType type,
    const std::vector<int>& n
) const
{
    std::vector<std::array<int, 2>> edges;

    switch (type) {
    case CellType::Line2:
        if (n.size() == 2) {
            edges.push_back({n[0], n[1]});
        }
        break;

    case CellType::Triangle3:
        if (n.size() == 3) {
            edges.push_back({n[0], n[1]});
            edges.push_back({n[1], n[2]});
            edges.push_back({n[2], n[0]});
        }
        break;

    case CellType::Quad4:
        if (n.size() == 4) {
            edges.push_back({n[0], n[1]});
            edges.push_back({n[1], n[2]});
            edges.push_back({n[2], n[3]});
            edges.push_back({n[3], n[0]});
        }
        break;

    case CellType::Tetra4:
        if (n.size() == 4) {
            edges.push_back({n[0], n[1]});
            edges.push_back({n[1], n[2]});
            edges.push_back({n[2], n[0]});

            edges.push_back({n[0], n[3]});
            edges.push_back({n[1], n[3]});
            edges.push_back({n[2], n[3]});
        }
        break;

    case CellType::Hex8:
        if (n.size() == 8) {
            edges.push_back({n[0], n[1]});
            edges.push_back({n[1], n[2]});
            edges.push_back({n[2], n[3]});
            edges.push_back({n[3], n[0]});

            edges.push_back({n[4], n[5]});
            edges.push_back({n[5], n[6]});
            edges.push_back({n[6], n[7]});
            edges.push_back({n[7], n[4]});

            edges.push_back({n[0], n[4]});
            edges.push_back({n[1], n[5]});
            edges.push_back({n[2], n[6]});
            edges.push_back({n[3], n[7]});
        }
        break;

    case CellType::Prism6:
        if (n.size() == 6) {
            edges.push_back({n[0], n[1]});
            edges.push_back({n[1], n[2]});
            edges.push_back({n[2], n[0]});

            edges.push_back({n[3], n[4]});
            edges.push_back({n[4], n[5]});
            edges.push_back({n[5], n[3]});

            edges.push_back({n[0], n[3]});
            edges.push_back({n[1], n[4]});
            edges.push_back({n[2], n[5]});
        }
        break;

    case CellType::Pyramid5:
        if (n.size() == 5) {
            edges.push_back({n[0], n[1]});
            edges.push_back({n[1], n[2]});
            edges.push_back({n[2], n[3]});
            edges.push_back({n[3], n[0]});

            edges.push_back({n[0], n[4]});
            edges.push_back({n[1], n[4]});
            edges.push_back({n[2], n[4]});
            edges.push_back({n[3], n[4]});
        }
        break;

    default:
        break;
    }

    return edges;
}

} // namespace OpenCAX
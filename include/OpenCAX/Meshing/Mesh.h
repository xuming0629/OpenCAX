#pragma once

#include <OpenCAX/Meshing/MeshTypes.h>

#include <vector>
#include <cstddef>

namespace OpenCAX
{

class Mesh
{
public:
    void clear();

    MeshInfo& info();
    const MeshInfo& info() const;

    std::vector<MeshNode>& nodes();
    std::vector<MeshCell>& cells();

    const std::vector<MeshNode>& nodes() const;
    const std::vector<MeshCell>& cells() const;

    int add_node(double x, double y, double z = 0.0);
    int add_cell(CellType type, const std::vector<int>& node_ids);

    std::size_t num_nodes() const;
    std::size_t num_cells() const;

    bool empty() const;

private:
    MeshInfo info_;
    std::vector<MeshNode> nodes_;
    std::vector<MeshCell> cells_;
};

}
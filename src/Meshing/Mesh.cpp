#include <OpenCAX/Meshing/Mesh.h>

namespace OpenCAX
{

void Mesh::clear()
{
    info_ = MeshInfo{};
    nodes_.clear();
    cells_.clear();
}

MeshInfo& Mesh::info()
{
    return info_;
}

const MeshInfo& Mesh::info() const
{
    return info_;
}

std::vector<MeshNode>& Mesh::nodes()
{
    return nodes_;
}

std::vector<MeshCell>& Mesh::cells()
{
    return cells_;
}

const std::vector<MeshNode>& Mesh::nodes() const
{
    return nodes_;
}

const std::vector<MeshCell>& Mesh::cells() const
{
    return cells_;
}

int Mesh::add_node(double x, double y, double z)
{
    nodes_.push_back({x, y, z});
    return static_cast<int>(nodes_.size()) - 1;
}

int Mesh::add_cell(CellType type, const std::vector<int>& node_ids)
{
    MeshCell cell;
    cell.type = type;
    cell.node_ids = node_ids;

    cells_.push_back(cell);
    return static_cast<int>(cells_.size()) - 1;
}

std::size_t Mesh::num_nodes() const
{
    return nodes_.size();
}

std::size_t Mesh::num_cells() const
{
    return cells_.size();
}

bool Mesh::empty() const
{
    return nodes_.empty() || cells_.empty();
}

}
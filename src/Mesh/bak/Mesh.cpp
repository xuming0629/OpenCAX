#include <OpenCAX/Mesh/Mesh.h>

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

int Mesh::add_node(
    double x,
    double y,
    double z
)
{
    MeshNode node;
    node.id = static_cast<int>(nodes_.size());
    node.x = x;
    node.y = y;
    node.z = z;

    nodes_.push_back(node);

    return node.id;
}

int Mesh::add_cell(
    CellType type,
    const std::vector<int>& node_ids
)
{
    MeshCell cell;
    cell.id = static_cast<int>(cells_.size());
    cell.type = type;
    cell.node_ids = node_ids;

    cells_.push_back(cell);

    return cell.id;
}

int Mesh::add_cell(
    CellType type,
    const std::vector<int>& node_ids,
    int physical_id,
    int material_id,
    int region_id
)
{
    MeshCell cell;
    cell.id = static_cast<int>(cells_.size());
    cell.type = type;
    cell.node_ids = node_ids;
    cell.physical_id = physical_id;
    cell.material_id = material_id;
    cell.region_id = region_id;

    cells_.push_back(cell);

    return cell.id;
}

bool Mesh::valid_node_id(
    int node_id
) const
{
    return node_id >= 0 &&
           node_id < static_cast<int>(nodes_.size());
}

bool Mesh::valid_cell_id(
    int cell_id
) const
{
    return cell_id >= 0 &&
           cell_id < static_cast<int>(cells_.size());
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

} // namespace OpenCAX
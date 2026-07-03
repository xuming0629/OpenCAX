/**
 * @file Mesh.cpp
 * @brief OpenCAX 通用网格基础容器实现
 *
 * 本文件实现 Mesh 类的基础数据管理接口。
 *
 * Mesh 只负责保存网格节点、网格单元和网格元信息。
 * 复杂拓扑关系由 MeshTopology 构建。
 */

#include <OpenCAX/Mesh/Mesh.h>
#include <OpenCAX/Mesh/CellTopology.h>

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

const std::vector<MeshNode>& Mesh::nodes() const
{
    return nodes_;
}

std::vector<MeshCell>& Mesh::cells()
{
    return cells_;
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
    const int node_id = static_cast<int>(nodes_.size());

    MeshNode node;
    node.id = node_id;
    node.x = x;
    node.y = y;
    node.z = z;

    nodes_.push_back(node);

    return node_id;
}

int Mesh::add_node(
    double x,
    double y,
    double z,
    int physical_id,
    int boundary_id
)
{
    const int node_id = static_cast<int>(nodes_.size());

    MeshNode node;
    node.id = node_id;
    node.x = x;
    node.y = y;
    node.z = z;
    node.physical_id = physical_id;
    node.boundary_id = boundary_id;

    nodes_.push_back(node);

    return node_id;
}

int Mesh::add_cell(
    CellType type,
    const std::vector<int>& node_ids
)
{
    const int cell_id = static_cast<int>(cells_.size());

    MeshCell cell;
    cell.id = cell_id;
    cell.type = type;
    cell.node_ids = node_ids;
    cell.order = CellTopology::order(type);

    cells_.push_back(cell);

    return cell_id;
}

int Mesh::add_cell(
    CellType type,
    const std::vector<int>& node_ids,
    int physical_id,
    int material_id,
    int region_id
)
{
    const int cell_id = static_cast<int>(cells_.size());

    MeshCell cell;
    cell.id = cell_id;
    cell.type = type;
    cell.node_ids = node_ids;
    cell.order = CellTopology::order(type);
    cell.physical_id = physical_id;
    cell.material_id = material_id;
    cell.region_id = region_id;

    cells_.push_back(cell);

    return cell_id;
}

bool Mesh::valid_node_id(
    int node_id
) const
{
    return node_id >= 0 &&
           static_cast<std::size_t>(node_id) < nodes_.size();
}

bool Mesh::valid_cell_id(
    int cell_id
) const
{
    return cell_id >= 0 &&
           static_cast<std::size_t>(cell_id) < cells_.size();
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
    return nodes_.empty() && cells_.empty();
}

bool Mesh::validate() const
{
    for (std::size_t i = 0; i < nodes_.size(); ++i)
    {
        if (nodes_[i].id != static_cast<int>(i))
        {
            return false;
        }
    }

    for (std::size_t i = 0; i < cells_.size(); ++i)
    {
        const MeshCell& cell = cells_[i];

        if (cell.id != static_cast<int>(i))
        {
            return false;
        }

        if (cell.type == CellType::Unknown)
        {
            return false;
        }

        for (int node_id : cell.node_ids)
        {
            if (!valid_node_id(node_id))
            {
                return false;
            }
        }
    }

    return true;
}

} // namespace OpenCAX
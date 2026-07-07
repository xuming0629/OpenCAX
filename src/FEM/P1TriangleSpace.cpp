#include <OpenCAX/FEM/P1TriangleSpace.h>

#include <OpenCAX/Mesh/MeshTypes.h>

#include <stdexcept>

namespace OpenCAX
{

P1TriangleSpace::P1TriangleSpace(
    const TriangleMesh& mesh,
    const MeshTopology& topology
)
    : mesh_(mesh),
      topology_(&topology)
{
}

P1TriangleSpace::P1TriangleSpace(
    const TriangleMesh& mesh
)
    : mesh_(mesh)
{
    owned_topology_.build(mesh_);
    topology_ = &owned_topology_;
}

const TriangleMesh& P1TriangleSpace::mesh() const
{
    return mesh_;
}

const MeshTopology& P1TriangleSpace::topology() const
{
    return *topology_;
}

std::string P1TriangleSpace::name() const
{
    return "P1TriangleSpace";
}

FEFamily P1TriangleSpace::family() const
{
    return FEFamily::Lagrange;
}

FEOrder P1TriangleSpace::order() const
{
    return FEOrder::P1;
}

std::size_t P1TriangleSpace::numDofs() const
{
    return mesh_.num_nodes();
}

std::size_t P1TriangleSpace::numCells() const
{
    return mesh_.num_cells();
}

std::size_t P1TriangleSpace::localDofCount() const
{
    return 3;
}

std::vector<int> P1TriangleSpace::cellDofs(
    std::size_t cell_id
) const
{
    const auto& cells = mesh_.cells();

    if (cell_id >= cells.size())
    {
        throw std::out_of_range(
            "[OpenCAX::P1TriangleSpace] cell_id out of range."
        );
    }

    const MeshCell& cell = cells[cell_id];

    if (cell.type != CellType::Triangle3)
    {
        throw std::runtime_error(
            "[OpenCAX::P1TriangleSpace] only Triangle3 cell is supported."
        );
    }

    if (cell.node_ids.size() != 3)
    {
        throw std::runtime_error(
            "[OpenCAX::P1TriangleSpace] Triangle3 cell must have 3 node ids."
        );
    }

    return {
        cell.node_ids[0],
        cell.node_ids[1],
        cell.node_ids[2]
    };
}

std::vector<int> P1TriangleSpace::boundaryDofs() const
{
    if (topology_ == nullptr)
    {
        throw std::runtime_error(
            "[OpenCAX::P1TriangleSpace] topology is null."
        );
    }

    return topology_->boundary_nodes();
}

std::array<double, 2> P1TriangleSpace::dofPoint(
    int dof_id
) const
{
    if (dof_id < 0)
    {
        throw std::out_of_range(
            "[OpenCAX::P1TriangleSpace] dof_id is negative."
        );
    }

    const auto& nodes = mesh_.nodes();

    const std::size_t node_id =
        static_cast<std::size_t>(dof_id);

    if (node_id >= nodes.size())
    {
        throw std::out_of_range(
            "[OpenCAX::P1TriangleSpace] dof_id out of range."
        );
    }

    const MeshNode& node = nodes[node_id];

    return {
        node.x,
        node.y
    };
}

} // namespace OpenCAX
#include <OpenCAX/Meshing/Mesh.h>

namespace OpenCAX
{

void Mesh::clear()
{
    nodes_.clear();
    cells_.clear();
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

}
#pragma once

#include <vector>

namespace OpenCAX
{

struct MeshNode
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct MeshCell
{
    int type = 0;
    std::vector<int> node_ids;
};

class Mesh
{
public:
    void clear();

    std::vector<MeshNode>& nodes();
    std::vector<MeshCell>& cells();

    const std::vector<MeshNode>& nodes() const;
    const std::vector<MeshCell>& cells() const;

private:
    std::vector<MeshNode> nodes_;
    std::vector<MeshCell> cells_;
};

}
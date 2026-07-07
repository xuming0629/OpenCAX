#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/FunctionSpace.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API P2TriangleSpace : public FunctionSpace
{
public:
    explicit P2TriangleSpace(const TriangleMesh& mesh);
    P2TriangleSpace(const TriangleMesh& mesh, const MeshTopology& topology);

    // 预留接口：需要 MeshTopology 稳定提供 edge(edge_id)、cell_edges(cell_id)、boundary_edges() 后实现。
    std::string name() const override { return "P2TriangleSpace"; }
    FEFamily family() const override { return FEFamily::Lagrange; }
    FEOrder order() const override { return FEOrder::P2; }

    std::size_t numDofs() const override { return 0; }
    std::size_t numCells() const override { return 0; }
    std::size_t localDofCount() const override { return 6; }

    std::vector<int> cellDofs(std::size_t) const override { return {}; }
    std::vector<int> boundaryDofs() const override { return {}; }
    std::array<double, 2> dofPoint(int) const override { return {0.0, 0.0}; }
};

} // namespace OpenCAX

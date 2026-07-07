#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/FunctionSpace.h>
#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 三角形 P1 Lagrange 有限元空间
 *
 * 说明：
 *
 * - 每个网格节点对应一个自由度
 * - 每个 Triangle3 单元有 3 个局部自由度
 * - 边界自由度来自 MeshTopology::boundary_nodes()
 */
class OpenCAX_API P1TriangleSpace : public FunctionSpace
{
public:
    /**
     * @brief 推荐构造：外部传入 mesh + topology
     */
    P1TriangleSpace(
        const TriangleMesh& mesh,
        const MeshTopology& topology
    );

    /**
     * @brief 便捷构造：内部自动构建 topology
     */
    explicit P1TriangleSpace(
        const TriangleMesh& mesh
    );

    const TriangleMesh& mesh() const;

    const MeshTopology& topology() const;

    std::string name() const override;

    FEFamily family() const override;

    FEOrder order() const override;

    std::size_t numDofs() const override;

    std::size_t numCells() const override;

    std::size_t localDofCount() const override;

    std::vector<int> cellDofs(
        std::size_t cell_id
    ) const override;

    std::vector<int> boundaryDofs() const override;

    std::array<double, 2> dofPoint(
        int dof_id
    ) const override;

private:
    const TriangleMesh& mesh_;

    MeshTopology owned_topology_;
    const MeshTopology* topology_ = nullptr;
};

} // namespace OpenCAX
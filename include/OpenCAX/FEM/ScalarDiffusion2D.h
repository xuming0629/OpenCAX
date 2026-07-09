#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/PhysicsProblem2D.h>

#include <functional>

namespace OpenCAX
{

class OpenCAX_API ScalarDiffusion2D : public PhysicsProblem2D
{
public:
    explicit ScalarDiffusion2D(const P1TriangleSpace& space);

    void setCoefficient(double k);
    void setCoefficient(ScalarFunction2D k);
    void setSource(Function2D f);

    void addDirichletBC(BoundarySelector2D selector, Function2D value);
    void addNeumannBC(BoundarySelector2D selector, Function2D value);

    void assemble() override;
    void applyBoundaryConditions() override;

private:
    void assembleCell(std::size_t cell_id, std::vector<Eigen::Triplet<double>>& triplets);
    void applyNeumann();
    void applyDirichletSparse();

private:
    Function2D source_;
};

} // namespace OpenCAX

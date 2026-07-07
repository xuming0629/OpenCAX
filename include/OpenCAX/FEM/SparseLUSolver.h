#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/LinearSolver.h>

namespace OpenCAX
{

class OpenCAX_API SparseLUSolver : public LinearSolver
{
public:
    SparseLUSolver() = default;

    bool solve(LinearSystem& system) override;

    SolverType type() const override
    {
        return SolverType::SparseLU;
    }

    std::string name() const override
    {
        return "Eigen::SparseLU";
    }
};

} // namespace OpenCAX

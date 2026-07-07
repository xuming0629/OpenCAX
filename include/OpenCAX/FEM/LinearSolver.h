#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/FETypes.h>
#include <OpenCAX/FEM/LinearSystem.h>

#include <string>

namespace OpenCAX
{

class OpenCAX_API LinearSolver
{
public:
    virtual ~LinearSolver() = default;

    virtual bool solve(LinearSystem& system) = 0;
    virtual SolverType type() const = 0;
    virtual std::string name() const = 0;
};

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/FETypes.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API FunctionSpace
{
public:
    virtual ~FunctionSpace() = default;

    virtual std::string name() const = 0;
    virtual FEFamily family() const = 0;
    virtual FEOrder order() const = 0;

    virtual std::size_t numDofs() const = 0;
    virtual std::size_t numCells() const = 0;
    virtual std::size_t localDofCount() const = 0;

    virtual std::vector<int> cellDofs(std::size_t cell_id) const = 0;
    virtual std::vector<int> boundaryDofs() const = 0;
    virtual std::array<double, 2> dofPoint(int dof_id) const = 0;
};

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/FETypes.h>

#include <functional>
#include <utility>

namespace OpenCAX
{

using Function2D = std::function<double(double, double)>;

class OpenCAX_API BoundaryCondition2D
{
public:
    BoundaryCondition2D()
        : type_(BoundaryType::Dirichlet)
    {
        value_ = [](double, double) { return 0.0; };
    }

    BoundaryCondition2D(BoundaryType type, Function2D value)
        : type_(type),
          value_(std::move(value))
    {
    }

    BoundaryType type() const
    {
        return type_;
    }

    double value(double x, double y) const
    {
        return value_(x, y);
    }

private:
    BoundaryType type_;
    Function2D value_;
};

} // namespace OpenCAX

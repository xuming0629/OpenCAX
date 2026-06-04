#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

#include <vector>

namespace OpenCAX
{

class OpenCAX_API Surface
{
public:
    static Shape plane(
        const gp_Pnt& origin,
        const gp_Vec& normal,
        double width,
        double height
    );

    static Shape ruled(
        const Shape& edge1,
        const Shape& edge2
    );

    static Shape bsplineSurface(
        const std::vector<std::vector<gp_Pnt>>& points
    );
};

} // namespace OpenCAX
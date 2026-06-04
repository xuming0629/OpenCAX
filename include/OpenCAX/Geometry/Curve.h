#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <gp_Pnt.hxx>

#include <vector>

namespace OpenCAX
{

class OpenCAX_API Curve
{
public:
    static Shape line(
        const gp_Pnt& p1,
        const gp_Pnt& p2
    );

    static Shape circle(
        const gp_Pnt& center,
        double radius
    );

    static Shape arc(
        const gp_Pnt& p1,
        const gp_Pnt& p2,
        const gp_Pnt& p3
    );

    static Shape bspline(
        const std::vector<gp_Pnt>& points
    );

    static Shape polyline(
        const std::vector<gp_Pnt>& points,
        bool closed = false
    );
};

} // namespace OpenCAX
#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>

namespace OpenCAX
{

class OpenCAX_API GeometryOps
{
public:

    static Shape fuse(
        const Shape& a,
        const Shape& b
    );

    static Shape cut(
        const Shape& a,
        const Shape& b
    );

    static Shape common(
        const Shape& a,
        const Shape& b
    );

    static Shape section(
        const Shape& a,
        const Shape& b
    );

    static Shape translate(
        const Shape& shape,
        double dx,
        double dy,
        double dz
    );

    static Shape rotate(
        const Shape& shape,
        const gp_Ax1& axis,
        double angleRad
    );

    static Shape scale(
        const Shape& shape,
        const gp_Pnt& center,
        double factor
    );

    static Shape mirrorPoint(
        const Shape& shape,
        const gp_Pnt& point
    );

    static Shape mirrorAxis(
        const Shape& shape,
        const gp_Ax1& axis
    );

    static Shape mirrorPlane(
        const Shape& shape,
        const gp_Pnt& point,
        const gp_Dir& normal
    );

    static Shape fillet(
        const Shape& shape,
        double radius
    );

    static Shape chamfer(
        const Shape& shape,
        double distance
    );
};

} // namespace OpenCAX
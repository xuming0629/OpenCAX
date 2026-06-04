#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>

namespace OpenCAX
{

class OpenCAX_API Transform
{
public:
    static Shape translate(
        const Shape& shape,
        const gp_Vec& vec
    );

    static Shape rotate(
        const Shape& shape,
        const gp_Ax1& axis,
        double angle_rad
    );

    static Shape scale(
        const Shape& shape,
        const gp_Pnt& center,
        double factor
    );

    static Shape mirror(
        const Shape& shape,
        const gp_Ax2& plane
    );
};

} // namespace OpenCAX
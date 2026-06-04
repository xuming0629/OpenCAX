#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <gp_Vec.hxx>

namespace OpenCAX
{

class OpenCAX_API SolidBuilder
{
public:
    static Shape extrude(
        const Shape& profile,
        const gp_Vec& direction
    );

    static Shape revolve(
        const Shape& profile,
        const gp_Ax1& axis,
        double angle_rad
    );

    static Shape makeSolidFromShell(
        const Shape& shell
    );
};

} // namespace OpenCAX
#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

namespace OpenCAX
{

class OpenCAX_API Primitive
{
public:
    static Shape box(double dx, double dy, double dz);

    static Shape box(
        const gp_Pnt& pmin,
        const gp_Pnt& pmax
    );

    static Shape sphere(double radius);

    static Shape sphere(
        const gp_Pnt& center,
        double radius
    );

    static Shape cylinder(
        double radius,
        double height
    );

    static Shape cylinder(
        const gp_Pnt& base,
        const gp_Vec& axis,
        double radius,
        double height
    );

    static Shape cone(
        double r1,
        double r2,
        double height
    );

    static Shape torus(
        double major_radius,
        double minor_radius
    );
};

} // namespace OpenCAX
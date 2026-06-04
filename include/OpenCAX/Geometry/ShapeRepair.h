#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

namespace OpenCAX
{

class OpenCAX_API ShapeRepair
{
public:
    static Shape sew(
        const Shape& shape,
        double tolerance = 1.0e-6
    );

    static Shape fixShape(
        const Shape& shape
    );

    static Shape removeSmallEdges(
        const Shape& shape,
        double tolerance = 1.0e-6
    );

    static Shape clean(
        const Shape& shape
    );
};

} // namespace OpenCAX
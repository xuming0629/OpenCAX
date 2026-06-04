#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <vector>

namespace OpenCAX
{

class OpenCAX_API Feature
{
public:
    static Shape filletAllEdges(
        const Shape& shape,
        double radius
    );

    static Shape chamferAllEdges(
        const Shape& shape,
        double distance
    );

    static Shape shell(
        const Shape& shape,
        double offset
    );
};

} // namespace OpenCAX
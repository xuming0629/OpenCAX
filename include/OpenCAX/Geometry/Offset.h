#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

namespace OpenCAX
{

class OpenCAX_API Offset
{
public:
    static Shape offsetShape(
        const Shape& shape,
        double distance
    );

    static Shape offsetFace(
        const Shape& face,
        double distance
    );
};

} // namespace OpenCAX
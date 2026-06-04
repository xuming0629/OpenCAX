#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

namespace OpenCAX
{

class OpenCAX_API FaceBuilder
{
public:
    static Shape fromWire(const Shape& wire);

    static Shape fromOuterAndInnerWires(
        const Shape& outer_wire,
        const std::vector<Shape>& inner_wires
    );
};

} // namespace OpenCAX
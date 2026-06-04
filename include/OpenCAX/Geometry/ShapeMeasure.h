#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

namespace OpenCAX
{

class OpenCAX_API ShapeMeasure
{
public:
    static double length(const Shape& edge_or_wire);
    static double area(const Shape& face_or_shell);
    static double volume(const Shape& solid);
};

} // namespace OpenCAX
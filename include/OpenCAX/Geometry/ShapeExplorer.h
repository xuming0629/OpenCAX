#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <vector>

namespace OpenCAX
{

class OpenCAX_API ShapeExplorer
{
public:
    static std::vector<Shape> vertices(const Shape& shape);
    static std::vector<Shape> edges(const Shape& shape);
    static std::vector<Shape> wires(const Shape& shape);
    static std::vector<Shape> faces(const Shape& shape);
    static std::vector<Shape> shells(const Shape& shape);
    static std::vector<Shape> solids(const Shape& shape);
};

} // namespace OpenCAX
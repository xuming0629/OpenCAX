#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <vector>

namespace OpenCAX
{

class OpenCAX_API WireBuilder
{
public:
    WireBuilder& addEdge(const Shape& edge);

    Shape build() const;

    static Shape fromEdges(
        const std::vector<Shape>& edges
    );

private:
    std::vector<Shape> edges_;
};

} // namespace OpenCAX
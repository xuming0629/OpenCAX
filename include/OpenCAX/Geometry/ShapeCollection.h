#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <vector>

namespace OpenCAX
{

class OpenCAX_API ShapeCollection
{
public:
    void add(const Shape& shape);
    void clear();

    size_t size() const;
    bool empty() const;

    Shape at(size_t index) const;

    Shape compound() const;

    const std::vector<Shape>& shapes() const;

private:
    std::vector<Shape> shapes_;
};

} // namespace OpenCAX
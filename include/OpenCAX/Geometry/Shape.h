#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/ShapeTypes.h>

#include <TopoDS_Shape.hxx>

namespace OpenCAX
{

class OpenCAX_API Shape
{
public:
    Shape();
    explicit Shape(const TopoDS_Shape& shape);

    bool isNull() const;
    bool isValid() const;

    ShapeType type() const;

    TopoDS_Shape& occ();
    const TopoDS_Shape& occ() const;

private:
    TopoDS_Shape shape_;
};

} // namespace OpenCAX
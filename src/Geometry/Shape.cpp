#include <OpenCAX/Geometry/Shape.h>

#include <BRepCheck_Analyzer.hxx>
#include <TopAbs_ShapeEnum.hxx>

namespace OpenCAX
{

Shape::Shape()
{
}

Shape::Shape(const TopoDS_Shape& shape)
    : shape_(shape)
{
}

bool Shape::isNull() const
{
    return shape_.IsNull();
}

bool Shape::isValid() const
{
    if (shape_.IsNull()) {
        return false;
    }

    BRepCheck_Analyzer analyzer(shape_);
    return analyzer.IsValid();
}

ShapeType Shape::type() const
{
    if (shape_.IsNull()) {
        return ShapeType::Unknown;
    }

    switch (shape_.ShapeType()) {
    case TopAbs_VERTEX:
        return ShapeType::Vertex;
    case TopAbs_EDGE:
        return ShapeType::Edge;
    case TopAbs_WIRE:
        return ShapeType::Wire;
    case TopAbs_FACE:
        return ShapeType::Face;
    case TopAbs_SHELL:
        return ShapeType::Shell;
    case TopAbs_SOLID:
        return ShapeType::Solid;
    case TopAbs_COMPOUND:
        return ShapeType::Compound;
    case TopAbs_COMPSOLID:
        return ShapeType::CompoundSolid;
    default:
        return ShapeType::Unknown;
    }
}

std::string Shape::typeName() const
{
    switch (type()) {
    case ShapeType::Vertex:
        return "Vertex";
    case ShapeType::Edge:
        return "Edge";
    case ShapeType::Wire:
        return "Wire";
    case ShapeType::Face:
        return "Face";
    case ShapeType::Shell:
        return "Shell";
    case ShapeType::Solid:
        return "Solid";
    case ShapeType::Compound:
        return "Compound";
    case ShapeType::CompoundSolid:
        return "CompoundSolid";
    default:
        return "Unknown";
    }
}

void Shape::clear()
{
    shape_.Nullify();
}

void Shape::setOcc(const TopoDS_Shape& shape)
{
    shape_ = shape;
}

TopoDS_Shape& Shape::occ()
{
    return shape_;
}

const TopoDS_Shape& Shape::occ() const
{
    return shape_;
}

} // namespace OpenCAX

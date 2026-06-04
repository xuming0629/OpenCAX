#pragma once

#include <OpenCAX/Geometry/ShapeTypes.h>

#include <TopoDS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>

namespace OpenCAX::internal
{

ShapeType toOpenCAXShapeType(TopAbs_ShapeEnum type);

TopAbs_ShapeEnum toOccShapeType(ShapeType type);

bool isNull(const TopoDS_Shape& shape);

} // namespace OpenCAX::internal
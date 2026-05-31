#include <OpenCAX/Geometry/ShapeFactory.h>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>

namespace OpenCAX
{

TopoDS_Shape ShapeFactory::makeBox(double dx, double dy, double dz)
{
    return BRepPrimAPI_MakeBox(dx, dy, dz).Shape();
}

TopoDS_Shape ShapeFactory::makeSphere(double radius)
{
    return BRepPrimAPI_MakeSphere(radius).Shape();
}

}
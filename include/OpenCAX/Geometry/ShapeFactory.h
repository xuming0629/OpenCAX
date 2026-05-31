#pragma once

#include <TopoDS_Shape.hxx>

namespace OpenCAX
{

class ShapeFactory
{
public:
    static TopoDS_Shape makeBox(double dx, double dy, double dz);
    static TopoDS_Shape makeSphere(double radius);
};

}
#include <OpenCAX/Geometry/GeometryBuilder.h>

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Vec.hxx>

#include <stdexcept>

namespace OpenCAX
{

Shape GeometryBuilder::makeVertex(double x, double y, double z)
{
    return makeVertex(gp_Pnt(x, y, z));
}

Shape GeometryBuilder::makeVertex(const gp_Pnt& p)
{
    BRepBuilderAPI_MakeVertex builder(p);
    return Shape(builder.Shape());
}

Shape GeometryBuilder::makeLine(const gp_Pnt& p1, const gp_Pnt& p2)
{
    BRepBuilderAPI_MakeEdge builder(p1, p2);

    if (!builder.IsDone()) {
        throw std::runtime_error("GeometryBuilder::makeLine failed.");
    }

    return Shape(builder.Edge());
}

Shape GeometryBuilder::makePolyline(const std::vector<gp_Pnt>& points, bool closed)
{
    if (points.size() < 2) {
        throw std::runtime_error("GeometryBuilder::makePolyline requires at least two points.");
    }

    BRepBuilderAPI_MakeWire wireBuilder;

    for (size_t i = 0; i + 1 < points.size(); ++i) {
        BRepBuilderAPI_MakeEdge edgeBuilder(points[i], points[i + 1]);
        if (!edgeBuilder.IsDone()) {
            throw std::runtime_error("GeometryBuilder::makePolyline failed to create edge.");
        }
        wireBuilder.Add(edgeBuilder.Edge());
    }

    if (closed) {
        BRepBuilderAPI_MakeEdge edgeBuilder(points.back(), points.front());
        if (!edgeBuilder.IsDone()) {
            throw std::runtime_error("GeometryBuilder::makePolyline failed to create closing edge.");
        }
        wireBuilder.Add(edgeBuilder.Edge());
    }

    if (!wireBuilder.IsDone()) {
        throw std::runtime_error("GeometryBuilder::makePolyline failed.");
    }

    return Shape(wireBuilder.Wire());
}

Shape GeometryBuilder::makeCircle(
    const gp_Pnt& center,
    double radius,
    const gp_Dir& normal
)
{
    if (radius <= 0.0) {
        throw std::runtime_error("Circle radius must be positive.");
    }

    Handle(Geom_Circle) circle = new Geom_Circle(gp_Ax2(center, normal), radius);
    BRepBuilderAPI_MakeEdge builder(circle);

    if (!builder.IsDone()) {
        throw std::runtime_error("GeometryBuilder::makeCircle failed.");
    }

    return Shape(builder.Edge());
}

Shape GeometryBuilder::makeArc(
    const gp_Pnt& p1,
    const gp_Pnt& p2,
    const gp_Pnt& p3
)
{
    Handle(Geom_TrimmedCurve) arc = GC_MakeArcOfCircle(p1, p2, p3);
    BRepBuilderAPI_MakeEdge builder(arc);

    if (!builder.IsDone()) {
        throw std::runtime_error("GeometryBuilder::makeArc failed.");
    }

    return Shape(builder.Edge());
}

Shape GeometryBuilder::makeRectangle(double width, double height)
{
    return makeRectangle(gp_Pnt(0.0, 0.0, 0.0), width, height);
}

Shape GeometryBuilder::makeRectangle(const gp_Pnt& origin, double width, double height)
{
    if (width <= 0.0 || height <= 0.0) {
        throw std::runtime_error("Rectangle width and height must be positive.");
    }

    std::vector<gp_Pnt> points;
    points.emplace_back(origin.X(), origin.Y(), origin.Z());
    points.emplace_back(origin.X() + width, origin.Y(), origin.Z());
    points.emplace_back(origin.X() + width, origin.Y() + height, origin.Z());
    points.emplace_back(origin.X(), origin.Y() + height, origin.Z());

    return makePolyline(points, true);
}

Shape GeometryBuilder::makeFaceFromWire(const Shape& wire)
{
    if (wire.isNull()) {
        throw std::runtime_error("makeFaceFromWire input wire is null.");
    }

    BRepBuilderAPI_MakeFace builder(TopoDS::Wire(wire.occ()));

    if (!builder.IsDone()) {
        throw std::runtime_error("GeometryBuilder::makeFaceFromWire failed.");
    }

    return Shape(builder.Face());
}

Shape GeometryBuilder::makePlaneFace(double width, double height)
{
    return makeFaceFromWire(makeRectangle(width, height));
}

Shape GeometryBuilder::makeBox(double dx, double dy, double dz)
{
    if (dx <= 0.0 || dy <= 0.0 || dz <= 0.0) {
        throw std::runtime_error("Box dimensions must be positive.");
    }

    return Shape(BRepPrimAPI_MakeBox(dx, dy, dz).Shape());
}

Shape GeometryBuilder::makeBox(const gp_Pnt& origin, double dx, double dy, double dz)
{
    if (dx <= 0.0 || dy <= 0.0 || dz <= 0.0) {
        throw std::runtime_error("Box dimensions must be positive.");
    }

    return Shape(BRepPrimAPI_MakeBox(origin, dx, dy, dz).Shape());
}

Shape GeometryBuilder::makeSphere(double radius)
{
    if (radius <= 0.0) {
        throw std::runtime_error("Sphere radius must be positive.");
    }

    return Shape(BRepPrimAPI_MakeSphere(radius).Shape());
}

Shape GeometryBuilder::makeSphere(const gp_Pnt& center, double radius)
{
    if (radius <= 0.0) {
        throw std::runtime_error("Sphere radius must be positive.");
    }

    return Shape(BRepPrimAPI_MakeSphere(center, radius).Shape());
}

Shape GeometryBuilder::makeCylinder(double radius, double height)
{
    if (radius <= 0.0 || height <= 0.0) {
        throw std::runtime_error("Cylinder radius and height must be positive.");
    }

    return Shape(BRepPrimAPI_MakeCylinder(radius, height).Shape());
}

Shape GeometryBuilder::makeCylinder(const gp_Ax2& axis, double radius, double height)
{
    if (radius <= 0.0 || height <= 0.0) {
        throw std::runtime_error("Cylinder radius and height must be positive.");
    }

    return Shape(BRepPrimAPI_MakeCylinder(axis, radius, height).Shape());
}

Shape GeometryBuilder::makeCone(double r1, double r2, double height)
{
    if (r1 < 0.0 || r2 < 0.0 || height <= 0.0) {
        throw std::runtime_error("Invalid cone parameters.");
    }

    return Shape(BRepPrimAPI_MakeCone(r1, r2, height).Shape());
}

Shape GeometryBuilder::makeTorus(double majorRadius, double minorRadius)
{
    if (majorRadius <= 0.0 || minorRadius <= 0.0) {
        throw std::runtime_error("Torus radii must be positive.");
    }

    return Shape(BRepPrimAPI_MakeTorus(majorRadius, minorRadius).Shape());
}

Shape GeometryBuilder::makePrism(const Shape& base, double dx, double dy, double dz)
{
    if (base.isNull()) {
        throw std::runtime_error("makePrism input shape is null.");
    }

    return Shape(BRepPrimAPI_MakePrism(base.occ(), gp_Vec(dx, dy, dz)).Shape());
}

Shape GeometryBuilder::makeRevolution(
    const Shape& profile,
    const gp_Ax1& axis,
    double angleRad
)
{
    if (profile.isNull()) {
        throw std::runtime_error("makeRevolution input shape is null.");
    }

    return Shape(BRepPrimAPI_MakeRevol(profile.occ(), axis, angleRad).Shape());
}

Shape GeometryBuilder::makeCompound(const std::vector<Shape>& shapes)
{
    TopoDS_Compound compound;
    BRep_Builder builder;
    builder.MakeCompound(compound);

    for (const auto& shape : shapes) {
        if (!shape.isNull()) {
            builder.Add(compound, shape.occ());
        }
    }

    return Shape(compound);
}

} // namespace OpenCAX

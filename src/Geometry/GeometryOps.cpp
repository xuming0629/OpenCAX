#include <OpenCAX/Geometry/GeometryOps.h>

#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Section.hxx>

#include <BRepBuilderAPI_Transform.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakeOffsetShape.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>

#include <BRepBuilderAPI_Sewing.hxx>

#include <ShapeFix_Shape.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>

#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>

#include <TopTools_ListOfShape.hxx>

#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax2.hxx>

#include <stdexcept>

namespace OpenCAX
{

namespace
{

void checkShape(
    const Shape& shape,
    const char* message
)
{
    if (shape.isNull()) {
        throw std::runtime_error(message);
    }
}

Shape applyTransform(
    const Shape& shape,
    const gp_Trsf& trsf
)
{
    checkShape(shape, "Transform input shape is null.");

    BRepBuilderAPI_Transform builder(
        shape.occ(),
        trsf,
        true
    );

    return Shape(builder.Shape());
}

} // namespace

Shape GeometryOps::fuse(
    const Shape& a,
    const Shape& b
)
{
    checkShape(a, "Fuse input A is null.");
    checkShape(b, "Fuse input B is null.");

    BRepAlgoAPI_Fuse algo(a.occ(), b.occ());
    algo.Build();

    if (!algo.IsDone()) {
        throw std::runtime_error("Boolean fuse failed.");
    }

    return Shape(algo.Shape());
}

Shape GeometryOps::cut(
    const Shape& a,
    const Shape& b
)
{
    checkShape(a, "Cut input A is null.");
    checkShape(b, "Cut input B is null.");

    BRepAlgoAPI_Cut algo(a.occ(), b.occ());
    algo.Build();

    if (!algo.IsDone()) {
        throw std::runtime_error("Boolean cut failed.");
    }

    return Shape(algo.Shape());
}

Shape GeometryOps::common(
    const Shape& a,
    const Shape& b
)
{
    checkShape(a, "Common input A is null.");
    checkShape(b, "Common input B is null.");

    BRepAlgoAPI_Common algo(a.occ(), b.occ());
    algo.Build();

    if (!algo.IsDone()) {
        throw std::runtime_error("Boolean common failed.");
    }

    return Shape(algo.Shape());
}

Shape GeometryOps::section(
    const Shape& a,
    const Shape& b
)
{
    checkShape(a, "Section input A is null.");
    checkShape(b, "Section input B is null.");

    BRepAlgoAPI_Section algo(a.occ(), b.occ());
    algo.Build();

    if (!algo.IsDone()) {
        throw std::runtime_error("Boolean section failed.");
    }

    return Shape(algo.Shape());
}

Shape GeometryOps::translate(
    const Shape& shape,
    double dx,
    double dy,
    double dz
)
{
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(dx, dy, dz));

    return applyTransform(shape, trsf);
}

Shape GeometryOps::rotate(
    const Shape& shape,
    const gp_Ax1& axis,
    double angleRad
)
{
    gp_Trsf trsf;
    trsf.SetRotation(axis, angleRad);

    return applyTransform(shape, trsf);
}

Shape GeometryOps::scale(
    const Shape& shape,
    const gp_Pnt& center,
    double factor
)
{
    gp_Trsf trsf;
    trsf.SetScale(center, factor);

    return applyTransform(shape, trsf);
}

Shape GeometryOps::mirrorPoint(
    const Shape& shape,
    const gp_Pnt& point
)
{
    gp_Trsf trsf;
    trsf.SetMirror(point);

    return applyTransform(shape, trsf);
}

Shape GeometryOps::mirrorAxis(
    const Shape& shape,
    const gp_Ax1& axis
)
{
    gp_Trsf trsf;
    trsf.SetMirror(axis);

    return applyTransform(shape, trsf);
}

Shape GeometryOps::mirrorPlane(
    const Shape& shape,
    const gp_Pnt& point,
    const gp_Dir& normal
)
{
    gp_Ax2 plane(point, normal);

    gp_Trsf trsf;
    trsf.SetMirror(plane);

    return applyTransform(shape, trsf);
}

Shape GeometryOps::fillet(
    const Shape& shape,
    double radius
)
{
    checkShape(shape, "Fillet input shape is null.");

    if (radius <= 0.0) {
        throw std::runtime_error("Fillet radius must be positive.");
    }

    BRepFilletAPI_MakeFillet fillet(shape.occ());

    for (TopExp_Explorer exp(shape.occ(), TopAbs_EDGE);
         exp.More();
         exp.Next())
    {
        TopoDS_Edge edge = TopoDS::Edge(exp.Current());
        fillet.Add(radius, edge);
    }

    fillet.Build();

    if (!fillet.IsDone()) {
        throw std::runtime_error("Fillet operation failed.");
    }

    return Shape(fillet.Shape());
}

Shape GeometryOps::chamfer(
    const Shape& shape,
    double distance
)
{
    checkShape(shape, "Chamfer input shape is null.");

    if (distance <= 0.0) {
        throw std::runtime_error("Chamfer distance must be positive.");
    }

    BRepFilletAPI_MakeChamfer chamfer(shape.occ());

    for (TopExp_Explorer exp(shape.occ(), TopAbs_EDGE);
         exp.More();
         exp.Next())
    {
        TopoDS_Edge edge = TopoDS::Edge(exp.Current());

        /*
         * OCC 7.9.2 supports:
         *
         *   Add(const TopoDS_Edge& E)
         *   Add(Standard_Real Dis, const TopoDS_Edge& E)
         *   Add(Standard_Real Dis1,
         *       Standard_Real Dis2,
         *       const TopoDS_Edge& E,
         *       const TopoDS_Face& F)
         *
         * It does NOT support:
         *
         *   Add(distance, edge, face)
         */
        chamfer.Add(distance, edge);
    }

    chamfer.Build();

    if (!chamfer.IsDone()) {
        throw std::runtime_error("Chamfer operation failed.");
    }

    return Shape(chamfer.Shape());
}

Shape GeometryOps::offset(
    const Shape& shape,
    double distance
)
{
    checkShape(shape, "Offset input shape is null.");

    BRepOffsetAPI_MakeOffsetShape offset;
    offset.PerformBySimple(
        shape.occ(),
        distance
    );

    if (!offset.IsDone()) {
        throw std::runtime_error("Offset operation failed.");
    }

    return Shape(offset.Shape());
}

Shape GeometryOps::shell(
    const Shape& shape,
    double thickness
)
{
    checkShape(shape, "Shell input shape is null.");

    TopTools_ListOfShape facesToRemove;

    BRepOffsetAPI_MakeThickSolid maker;

    maker.MakeThickSolidByJoin(
        shape.occ(),
        facesToRemove,
        thickness,
        1.0e-3
    );

    if (!maker.IsDone()) {
        throw std::runtime_error("Shell operation failed.");
    }

    return Shape(maker.Shape());
}

Shape GeometryOps::sew(
    const Shape& shape,
    double tolerance
)
{
    checkShape(shape, "Sew input shape is null.");

    BRepBuilderAPI_Sewing sewing(tolerance);
    sewing.Add(shape.occ());
    sewing.Perform();

    TopoDS_Shape sewed = sewing.SewedShape();

    if (sewed.IsNull()) {
        throw std::runtime_error("Sew operation failed.");
    }

    return Shape(sewed);
}

Shape GeometryOps::fix(
    const Shape& shape
)
{
    checkShape(shape, "Fix input shape is null.");

    ShapeFix_Shape fixer(shape.occ());
    fixer.Perform();

    TopoDS_Shape fixed = fixer.Shape();

    if (fixed.IsNull()) {
        throw std::runtime_error("Fix operation failed.");
    }

    return Shape(fixed);
}

Shape GeometryOps::unifySameDomain(
    const Shape& shape
)
{
    checkShape(shape, "UnifySameDomain input shape is null.");

    ShapeUpgrade_UnifySameDomain unify(
        shape.occ(),
        true,
        true,
        true
    );

    unify.Build();

    TopoDS_Shape unified = unify.Shape();

    if (unified.IsNull()) {
        throw std::runtime_error("UnifySameDomain operation failed.");
    }

    return Shape(unified);
}

} // namespace OpenCAX
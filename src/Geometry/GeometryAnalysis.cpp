#include <OpenCAX/Geometry/GeometryAnalysis.h>

#include <BRepAdaptor_Surface.hxx>
#include <BRepBndLib.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepGProp.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <GProp_GProps.hxx>
#include <GeomLProp_SLProps.hxx>
#include <TopAbs.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

#include <stdexcept>

namespace OpenCAX
{

double BoundingBox::dx() const
{
    return xmax - xmin;
}

double BoundingBox::dy() const
{
    return ymax - ymin;
}

double BoundingBox::dz() const
{
    return zmax - zmin;
}

namespace
{
void checkShape(const Shape& shape, const char* message)
{
    if (shape.isNull()) {
        throw std::runtime_error(message);
    }
}

std::vector<Shape> collectSubShapes(const Shape& shape, TopAbs_ShapeEnum type)
{
    checkShape(shape, "collectSubShapes input shape is null.");

    std::vector<Shape> result;

    for (TopExp_Explorer exp(shape.occ(), type); exp.More(); exp.Next()) {
        result.emplace_back(exp.Current());
    }

    return result;
}
}

BoundingBox GeometryAnalysis::boundingBox(const Shape& shape, double tolerance)
{
    checkShape(shape, "BoundingBox input shape is null.");

    Bnd_Box box;
    box.SetGap(tolerance);

    BRepBndLib::Add(shape.occ(), box);

    BoundingBox result;
    box.Get(
        result.xmin,
        result.ymin,
        result.zmin,
        result.xmax,
        result.ymax,
        result.zmax
    );

    return result;
}

double GeometryAnalysis::length(const Shape& shape)
{
    checkShape(shape, "Length input shape is null.");

    GProp_GProps props;
    BRepGProp::LinearProperties(shape.occ(), props);

    return props.Mass();
}

double GeometryAnalysis::area(const Shape& shape)
{
    checkShape(shape, "Area input shape is null.");

    GProp_GProps props;
    BRepGProp::SurfaceProperties(shape.occ(), props);

    return props.Mass();
}

double GeometryAnalysis::volume(const Shape& shape)
{
    checkShape(shape, "Volume input shape is null.");

    GProp_GProps props;
    BRepGProp::VolumeProperties(shape.occ(), props);

    return props.Mass();
}

gp_Pnt GeometryAnalysis::centerOfMass(const Shape& shape)
{
    checkShape(shape, "CenterOfMass input shape is null.");

    GProp_GProps props;

    if (shape.type() == ShapeType::Solid ||
        shape.type() == ShapeType::Compound ||
        shape.type() == ShapeType::CompoundSolid) {
        BRepGProp::VolumeProperties(shape.occ(), props);
    } else if (shape.type() == ShapeType::Face || shape.type() == ShapeType::Shell) {
        BRepGProp::SurfaceProperties(shape.occ(), props);
    } else {
        BRepGProp::LinearProperties(shape.occ(), props);
    }

    return props.CentreOfMass();
}

ShapeStats GeometryAnalysis::countSubShapes(const Shape& shape)
{
    checkShape(shape, "countSubShapes input shape is null.");

    ShapeStats stats;

    for (TopExp_Explorer exp(shape.occ(), TopAbs_VERTEX); exp.More(); exp.Next()) {
        ++stats.vertices;
    }

    for (TopExp_Explorer exp(shape.occ(), TopAbs_EDGE); exp.More(); exp.Next()) {
        ++stats.edges;
    }

    for (TopExp_Explorer exp(shape.occ(), TopAbs_WIRE); exp.More(); exp.Next()) {
        ++stats.wires;
    }

    for (TopExp_Explorer exp(shape.occ(), TopAbs_FACE); exp.More(); exp.Next()) {
        ++stats.faces;
    }

    for (TopExp_Explorer exp(shape.occ(), TopAbs_SHELL); exp.More(); exp.Next()) {
        ++stats.shells;
    }

    for (TopExp_Explorer exp(shape.occ(), TopAbs_SOLID); exp.More(); exp.Next()) {
        ++stats.solids;
    }

    for (TopExp_Explorer exp(shape.occ(), TopAbs_COMPOUND); exp.More(); exp.Next()) {
        ++stats.compounds;
    }

    return stats;
}

std::vector<Shape> GeometryAnalysis::vertices(const Shape& shape)
{
    return collectSubShapes(shape, TopAbs_VERTEX);
}

std::vector<Shape> GeometryAnalysis::edges(const Shape& shape)
{
    return collectSubShapes(shape, TopAbs_EDGE);
}

std::vector<Shape> GeometryAnalysis::wires(const Shape& shape)
{
    return collectSubShapes(shape, TopAbs_WIRE);
}

std::vector<Shape> GeometryAnalysis::faces(const Shape& shape)
{
    return collectSubShapes(shape, TopAbs_FACE);
}

std::vector<Shape> GeometryAnalysis::shells(const Shape& shape)
{
    return collectSubShapes(shape, TopAbs_SHELL);
}

std::vector<Shape> GeometryAnalysis::solids(const Shape& shape)
{
    return collectSubShapes(shape, TopAbs_SOLID);
}

bool GeometryAnalysis::isClosed(const Shape& shape)
{
    checkShape(shape, "isClosed input shape is null.");
    return shape.occ().Closed();
}

bool GeometryAnalysis::isValid(const Shape& shape)
{
    if (shape.isNull()) {
        return false;
    }

    BRepCheck_Analyzer analyzer(shape.occ());
    return analyzer.IsValid();
}

double GeometryAnalysis::distance(const Shape& a, const Shape& b)
{
    checkShape(a, "distance input A is null.");
    checkShape(b, "distance input B is null.");

    BRepExtrema_DistShapeShape dist(a.occ(), b.occ());
    dist.Perform();

    if (!dist.IsDone()) {
        throw std::runtime_error("Distance calculation failed.");
    }

    return dist.Value();
}

gp_Pnt GeometryAnalysis::vertexPoint(const Shape& vertex)
{
    if (vertex.type() != ShapeType::Vertex) {
        throw std::runtime_error("vertexPoint input is not vertex.");
    }

    return BRep_Tool::Pnt(TopoDS::Vertex(vertex.occ()));
}

gp_Pnt GeometryAnalysis::edgeStartPoint(const Shape& edge)
{
    if (edge.type() != ShapeType::Edge) {
        throw std::runtime_error("edgeStartPoint input is not edge.");
    }

    TopoDS_Vertex v1;
    TopoDS_Vertex v2;
    TopExp::Vertices(TopoDS::Edge(edge.occ()), v1, v2);

    return BRep_Tool::Pnt(v1);
}

gp_Pnt GeometryAnalysis::edgeEndPoint(const Shape& edge)
{
    if (edge.type() != ShapeType::Edge) {
        throw std::runtime_error("edgeEndPoint input is not edge.");
    }

    TopoDS_Vertex v1;
    TopoDS_Vertex v2;
    TopExp::Vertices(TopoDS::Edge(edge.occ()), v1, v2);

    return BRep_Tool::Pnt(v2);
}

gp_Vec GeometryAnalysis::faceNormal(const Shape& face)
{
    if (face.type() != ShapeType::Face) {
        throw std::runtime_error("faceNormal input is not face.");
    }

    TopoDS_Face occFace = TopoDS::Face(face.occ());
    BRepAdaptor_Surface surface(occFace);

    const double u = 0.5 * (surface.FirstUParameter() + surface.LastUParameter());
    const double v = 0.5 * (surface.FirstVParameter() + surface.LastVParameter());

    GeomLProp_SLProps props(surface.Surface().Surface(), u, v, 1, 1e-6);

    if (!props.IsNormalDefined()) {
        throw std::runtime_error("Face normal is not defined.");
    }

    gp_Dir normal = props.Normal();

    if (occFace.Orientation() == TopAbs_REVERSED) {
        normal.Reverse();
    }

    return gp_Vec(normal);
}

} // namespace OpenCAX

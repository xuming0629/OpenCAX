#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

#include <vector>

namespace OpenCAX
{

struct OpenCAX_API BoundingBox
{
    double xmin = 0.0;
    double ymin = 0.0;
    double zmin = 0.0;
    double xmax = 0.0;
    double ymax = 0.0;
    double zmax = 0.0;

    double dx() const;
    double dy() const;
    double dz() const;
};

struct OpenCAX_API ShapeStats
{
    int vertices = 0;
    int edges = 0;
    int wires = 0;
    int faces = 0;
    int shells = 0;
    int solids = 0;
    int compounds = 0;
};

/**
 * @brief 几何分析工具类
 */
class OpenCAX_API GeometryAnalysis
{
public:
    static BoundingBox boundingBox(const Shape& shape, double tolerance = 1e-7);

    static double length(const Shape& shape);
    static double area(const Shape& shape);
    static double volume(const Shape& shape);

    static gp_Pnt centerOfMass(const Shape& shape);

    static ShapeStats countSubShapes(const Shape& shape);

    static std::vector<Shape> vertices(const Shape& shape);
    static std::vector<Shape> edges(const Shape& shape);
    static std::vector<Shape> wires(const Shape& shape);
    static std::vector<Shape> faces(const Shape& shape);
    static std::vector<Shape> shells(const Shape& shape);
    static std::vector<Shape> solids(const Shape& shape);

    static bool isClosed(const Shape& shape);
    static bool isValid(const Shape& shape);

    static double distance(const Shape& a, const Shape& b);

    static gp_Pnt vertexPoint(const Shape& vertex);
    static gp_Pnt edgeStartPoint(const Shape& edge);
    static gp_Pnt edgeEndPoint(const Shape& edge);

    static gp_Vec faceNormal(const Shape& face);
};

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Geometry/Shape.h>

#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>

#include <vector>

namespace OpenCAX
{

/**
 * @brief 几何构造工具类
 *
 * 用于创建 OpenCAX 中常用的 OCC 拓扑几何对象。
 */
class OpenCAX_API GeometryBuilder
{
public:
    static Shape makeVertex(double x, double y, double z);
    static Shape makeVertex(const gp_Pnt& p);

    static Shape makeLine(const gp_Pnt& p1, const gp_Pnt& p2);
    static Shape makePolyline(const std::vector<gp_Pnt>& points, bool closed = false);

    static Shape makeCircle(
        const gp_Pnt& center,
        double radius,
        const gp_Dir& normal = gp_Dir(0.0, 0.0, 1.0)
    );

    static Shape makeArc(
        const gp_Pnt& p1,
        const gp_Pnt& p2,
        const gp_Pnt& p3
    );

    static Shape makeRectangle(double width, double height);
    static Shape makeRectangle(const gp_Pnt& origin, double width, double height);

    static Shape makeFaceFromWire(const Shape& wire);
    static Shape makePlaneFace(double width, double height);

    static Shape makeBox(double dx, double dy, double dz);
    static Shape makeBox(const gp_Pnt& origin, double dx, double dy, double dz);

    static Shape makeSphere(double radius);
    static Shape makeSphere(const gp_Pnt& center, double radius);

    static Shape makeCylinder(double radius, double height);
    static Shape makeCylinder(const gp_Ax2& axis, double radius, double height);

    static Shape makeCone(double r1, double r2, double height);
    static Shape makeTorus(double majorRadius, double minorRadius);

    static Shape makePrism(const Shape& base, double dx, double dy, double dz);

    static Shape makeRevolution(
        const Shape& profile,
        const gp_Ax1& axis,
        double angleRad
    );

    static Shape makeCompound(const std::vector<Shape>& shapes);
};

} // namespace OpenCAX

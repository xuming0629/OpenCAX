#include <OpenCAX/Geometry/GeometryModule.h>

#include <gp_Pnt.hxx>

#include <iostream>

int main()
{
    using namespace OpenCAX;

    Shape box = GeometryBuilder::makeBox(10.0, 10.0, 10.0);
    Shape sphere = GeometryBuilder::makeSphere(gp_Pnt(5.0, 5.0, 5.0), 4.0);

    Shape cutResult = GeometryOps::cut(box, sphere);
    Shape fixed = GeometryOps::fix(cutResult);
    Shape moved = GeometryOps::translate(fixed, 20.0, 0.0, 0.0);

    BoundingBox bbox = GeometryAnalysis::boundingBox(moved);
    ShapeStats stats = GeometryAnalysis::countSubShapes(moved);

    std::cout << "type: " << moved.typeName() << std::endl;
    std::cout << "valid: " << GeometryAnalysis::isValid(moved) << std::endl;

    std::cout << "bbox: "
              << bbox.xmin << ", "
              << bbox.ymin << ", "
              << bbox.zmin << " -> "
              << bbox.xmax << ", "
              << bbox.ymax << ", "
              << bbox.zmax << std::endl;

    std::cout << "volume: " << GeometryAnalysis::volume(moved) << std::endl;
    std::cout << "area: " << GeometryAnalysis::area(moved) << std::endl;

    std::cout << "vertices: " << stats.vertices << std::endl;
    std::cout << "edges: " << stats.edges << std::endl;
    std::cout << "faces: " << stats.faces << std::endl;
    std::cout << "solids: " << stats.solids << std::endl;

    GeometryIO::writeSTEP(moved, "geometry_result.step");
    GeometryIO::writeBREP(moved, "geometry_result.brep");
    GeometryIO::writeSTL(moved, "geometry_result.stl");

    return 0;
}

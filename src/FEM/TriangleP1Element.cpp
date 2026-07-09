#include <OpenCAX/FEM/TriangleP1Element.h>

#include <cmath>

namespace OpenCAX
{

bool TriangleP1Element::computeGeometry(
    const MeshNode& p0,
    const MeshNode& p1,
    const MeshNode& p2,
    TriangleP1Geometry& geo,
    double eps
)
{
    const double x0 = p0.x;
    const double y0 = p0.y;
    const double x1 = p1.x;
    const double y1 = p1.y;
    const double x2 = p2.x;
    const double y2 = p2.y;

    const double detJ = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
    geo.area = 0.5 * std::abs(detJ);

    if (geo.area < eps)
    {
        return false;
    }

    const double twoA = 2.0 * geo.area;

    geo.dphidx[0] = (y1 - y2) / twoA;
    geo.dphidx[1] = (y2 - y0) / twoA;
    geo.dphidx[2] = (y0 - y1) / twoA;

    geo.dphidy[0] = (x2 - x1) / twoA;
    geo.dphidy[1] = (x0 - x2) / twoA;
    geo.dphidy[2] = (x1 - x0) / twoA;

    geo.centroid = {(x0 + x1 + x2) / 3.0, (y0 + y1 + y2) / 3.0};

    return true;
}

} // namespace OpenCAX

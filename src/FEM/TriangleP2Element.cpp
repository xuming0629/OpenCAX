#include <OpenCAX/FEM/TriangleP2Element.h>

#include <cmath>

namespace OpenCAX
{

bool TriangleP2Element::computeGeometry(
    const MeshNode& p0,
    const MeshNode& p1,
    const MeshNode& p2,
    TriangleP2Geometry& geo,
    double eps
)
{
    const double detJ = (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
    geo.area = 0.5 * std::abs(detJ);
    geo.centroid = {(p0.x + p1.x + p2.x) / 3.0, (p0.y + p1.y + p2.y) / 3.0};
    return geo.area >= eps;
}

TriangleP2ShapeData TriangleP2Element::evaluate(
    const MeshNode& p0,
    const MeshNode& p1,
    const MeshNode& p2,
    const std::array<double, 3>& L
)
{
    TriangleP2ShapeData s;

    const double L1 = L[0];
    const double L2 = L[1];
    const double L3 = L[2];

    s.N[0] = L1 * (2.0 * L1 - 1.0);
    s.N[1] = L2 * (2.0 * L2 - 1.0);
    s.N[2] = L3 * (2.0 * L3 - 1.0);
    s.N[3] = 4.0 * L1 * L2;
    s.N[4] = 4.0 * L2 * L3;
    s.N[5] = 4.0 * L3 * L1;

    const double x0 = p0.x, y0 = p0.y;
    const double x1 = p1.x, y1 = p1.y;
    const double x2 = p2.x, y2 = p2.y;
    const double detJ = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
    const double area = 0.5 * std::abs(detJ);
    const double twoA = 2.0 * area;

    const double dLdx[3] = {
        (y1 - y2) / twoA,
        (y2 - y0) / twoA,
        (y0 - y1) / twoA
    };

    const double dLdy[3] = {
        (x2 - x1) / twoA,
        (x0 - x2) / twoA,
        (x1 - x0) / twoA
    };

    const double dNdL[6][3] = {
        {4.0 * L1 - 1.0, 0.0, 0.0},
        {0.0, 4.0 * L2 - 1.0, 0.0},
        {0.0, 0.0, 4.0 * L3 - 1.0},
        {4.0 * L2, 4.0 * L1, 0.0},
        {0.0, 4.0 * L3, 4.0 * L2},
        {4.0 * L3, 0.0, 4.0 * L1}
    };

    for (int a = 0; a < 6; ++a)
    {
        s.dNdx[a] = 0.0;
        s.dNdy[a] = 0.0;
        for (int k = 0; k < 3; ++k)
        {
            s.dNdx[a] += dNdL[a][k] * dLdx[k];
            s.dNdy[a] += dNdL[a][k] * dLdy[k];
        }
    }

    return s;
}

} // namespace OpenCAX

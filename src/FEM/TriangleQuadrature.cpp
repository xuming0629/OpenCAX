#include <OpenCAX/FEM/TriangleQuadrature.h>

namespace OpenCAX
{

std::vector<TriangleQuadraturePoint> TriangleQuadrature::degree1()
{
    return { TriangleQuadraturePoint{{1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}, 1.0} };
}

std::vector<TriangleQuadraturePoint> TriangleQuadrature::degree2()
{
    return {
        TriangleQuadraturePoint{{2.0 / 3.0, 1.0 / 6.0, 1.0 / 6.0}, 1.0 / 3.0},
        TriangleQuadraturePoint{{1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0}, 1.0 / 3.0},
        TriangleQuadraturePoint{{1.0 / 6.0, 1.0 / 6.0, 2.0 / 3.0}, 1.0 / 3.0}
    };
}

std::vector<TriangleQuadraturePoint> TriangleQuadrature::degree3()
{
    return {
        TriangleQuadraturePoint{{1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}, -9.0 / 16.0},
        TriangleQuadraturePoint{{0.6, 0.2, 0.2}, 25.0 / 48.0},
        TriangleQuadraturePoint{{0.2, 0.6, 0.2}, 25.0 / 48.0},
        TriangleQuadraturePoint{{0.2, 0.2, 0.6}, 25.0 / 48.0}
    };
}

std::vector<TriangleQuadraturePoint> TriangleQuadrature::create(int degree)
{
    if (degree <= 1) return degree1();
    if (degree == 2) return degree2();
    return degree3();
}

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Core/Config.h>

#include <array>
#include <vector>

namespace OpenCAX
{

struct OpenCAX_API TriangleQuadraturePoint
{
    std::array<double, 3> lambda = {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0};
    double weight = 1.0;
};

class OpenCAX_API TriangleQuadrature
{
public:
    static std::vector<TriangleQuadraturePoint> degree1();
    static std::vector<TriangleQuadraturePoint> degree2();
    static std::vector<TriangleQuadraturePoint> degree3();
    static std::vector<TriangleQuadraturePoint> create(int degree);
};

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/MeshTypes.h>

#include <array>

namespace OpenCAX
{

struct OpenCAX_API TriangleP1Geometry
{
    double area = 0.0;
    std::array<double, 2> centroid = {0.0, 0.0};
    std::array<double, 3> dphidx = {0.0, 0.0, 0.0};
    std::array<double, 3> dphidy = {0.0, 0.0, 0.0};
};

class OpenCAX_API TriangleP1Element
{
public:
    static bool computeGeometry(
        const MeshNode& p0,
        const MeshNode& p1,
        const MeshNode& p2,
        TriangleP1Geometry& geo,
        double eps = 1.0e-14
    );
};

} // namespace OpenCAX

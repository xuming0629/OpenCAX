#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/TriangleQuadrature.h>
#include <OpenCAX/Mesh/MeshTypes.h>

#include <array>
#include <vector>

namespace OpenCAX
{

struct OpenCAX_API TriangleP2ShapeData
{
    std::array<double, 6> N = {0, 0, 0, 0, 0, 0};
    std::array<double, 6> dNdx = {0, 0, 0, 0, 0, 0};
    std::array<double, 6> dNdy = {0, 0, 0, 0, 0, 0};
};

struct OpenCAX_API TriangleP2Geometry
{
    double area = 0.0;
    std::array<double, 2> centroid = {0.0, 0.0};
};

class OpenCAX_API TriangleP2Element
{
public:
    static bool computeGeometry(
        const MeshNode& p0,
        const MeshNode& p1,
        const MeshNode& p2,
        TriangleP2Geometry& geo,
        double eps = 1.0e-14
    );

    static TriangleP2ShapeData evaluate(
        const MeshNode& p0,
        const MeshNode& p1,
        const MeshNode& p2,
        const std::array<double, 3>& lambda
    );
};

} // namespace OpenCAX

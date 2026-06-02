#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Meshing/Mesh.h>
#include <OpenCAX/Meshing/MeshingOptions.h>

#include <TopoDS_Shape.hxx>

#include <array>
#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API TetgenMesher
{
public:
    static Mesh generateVolumeMesh(
        const TopoDS_Shape& shape,
        const MeshingOptions& options = MeshingOptions()
    );

    static Mesh generateVolumeMeshFromFile(
        const std::string& filename,
        const MeshingOptions& options = MeshingOptions()
    );

    static Mesh createBox(
        double lx,
        double ly,
        double lz,
        const MeshingOptions& options = MeshingOptions()
    );

    static Mesh createSphere(
        double radius,
        const MeshingOptions& options = MeshingOptions()
    );

    static Mesh createCylinder(
        double radius,
        double height,
        const MeshingOptions& options = MeshingOptions()
    );

private:
    static bool shapeToSurfaceTriangles(
        const TopoDS_Shape& shape,
        std::vector<std::array<double, 3>>& points,
        std::vector<std::array<int, 3>>& triangles,
        double deflection
    );

    static Mesh generateFromSurfaceTriangles(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 3>>& triangles,
        const MeshingOptions& options
    );
};

} // namespace OpenCAX
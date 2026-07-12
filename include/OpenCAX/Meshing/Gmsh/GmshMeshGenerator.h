#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Meshing/MeshGenerationOptions.h>

namespace OpenCAX
{

class OpenCAX_API GmshMeshGenerator
{
public:
    static TriangleMesh rectangle_2d(double xmin, double xmax, double ymin, double ymax, const MeshGenerationOptions& options);
    static TriangleMesh structured_rectangle_2d(double xmin, double xmax, double ymin, double ymax, int nx, int ny, const MeshGenerationOptions& options);
    static void finalize();
};

} // namespace OpenCAX

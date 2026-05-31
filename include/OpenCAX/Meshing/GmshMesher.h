#pragma once

#include <OpenCAX/Meshing/Mesh.h>

#include <TopoDS_Shape.hxx>
#include <string>

namespace OpenCAX
{

struct MeshingOptions
{
    double mesh_size_min = 0.1;
    double mesh_size_max = 0.1;
    bool second_order = false;
    bool optimize = true;
    bool verbose = false;
};

class GmshMesher
{
public:
    static Mesh generateSurfaceMesh(
        const TopoDS_Shape& shape,
        const MeshingOptions& options
    );

    static Mesh generateVolumeMesh(
        const TopoDS_Shape& shape,
        const MeshingOptions& options
    );

private:
    static Mesh generate(
        const TopoDS_Shape& shape,
        const MeshingOptions& options,
        int dim
    );
};

}
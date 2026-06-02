#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Meshing/Mesh.h>
#include <OpenCAX/Meshing/MeshingOptions.h>

#include <TopoDS_Shape.hxx>

namespace OpenCAX
{

class OpenCAX_API GmshMesher
{
public:
    static Mesh generateSurfaceMesh(
        const TopoDS_Shape& shape,
        const MeshingOptions& options = MeshingOptions()
    );

    static Mesh generateVolumeMesh(
        const TopoDS_Shape& shape,
        const MeshingOptions& options = MeshingOptions()
    );

private:
    static Mesh generate(
        const TopoDS_Shape& shape,
        const MeshingOptions& options,
        int dim
    );
};

} // namespace OpenCAX
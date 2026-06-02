#include <OpenCAX/Meshing/GmshMesher.h>
#include <OpenCAX/Meshing/MeshingOptions.h>
#include <OpenCAX/Visualization/VtkViewer.h>

#include <BRepPrimAPI_MakeBox.hxx>

using namespace OpenCAX;

int main()
{
    TopoDS_Shape shape = BRepPrimAPI_MakeBox(
        2.0,
        1.0,
        1.0
    ).Shape();

    MeshingOptions options;
    options.mesh_size_min = 0.1;
    options.mesh_size_max = 0.1;

    options.nx = 12;
    options.ny = 6;
    options.nz = 6;

    options.second_order = false;
    options.optimize = true;
    options.verbose = true;

    /*
        注意：
        六面体需要你的 GmshMesher 内部启用：
        - TransfiniteCurve
        - TransfiniteSurface
        - TransfiniteVolume
        - Recombine
    */

    GmshMesher mesher;
    Mesh mesh = mesher.generateVolumeMesh(shape, options);

    VtkViewer viewer;
    viewer.showMesh(mesh);

    return 0;
}
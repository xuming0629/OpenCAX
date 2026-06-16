#include <OpenCAX/Meshing/GmshMesher.h>
#include <OpenCAX/Meshing/MeshingOptions.h>
#include <OpenCAX/Visualization/VtkViewer.h>

#include <BRepPrimAPI_MakeSphere.hxx>
#include <gp_Pnt.hxx>

using namespace OpenCAX;

int main()
{
    TopoDS_Shape shape = BRepPrimAPI_MakeSphere(
        gp_Pnt(0, 0, 0),
        1.0
    ).Shape();

    MeshingOptions options;
    options.mesh_size_min = 0.03;
    options.mesh_size_max = 0.12;
    options.optimize = true;
    options.verbose = true;

    GmshMesher mesher;
    Mesh mesh = mesher.generateSurfaceMesh(shape, options);

    VtkViewer viewer;
    viewer.showMesh(mesh);

    return 0;
}
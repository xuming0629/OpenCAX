#include <OpenCAX/Meshing/GmshMesher.h>
#include <OpenCAX/Visualization/VtkViewer.h>

#include <BRepPrimAPI_MakeCylinder.hxx>

using namespace OpenCAX;

int main()
{
    TopoDS_Shape shape = BRepPrimAPI_MakeCylinder(1.0, 2.0).Shape();

    MeshingOptions opt;
    opt.mesh_size_min = 0.08;
    opt.mesh_size_max = 0.08;
    opt.recombine = true;
    opt.transfinite = false;
    opt.verbose = true;

    Mesh mesh = GmshMesher::generateSurfaceMesh(shape, opt);

    VtkViewer viewer;
    viewer.showMesh(mesh);

    return 0;
}
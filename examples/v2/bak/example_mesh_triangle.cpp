#include <OpenCAX/Meshing/GmshMesher.h>
#include <OpenCAX/Meshing/MeshingOptions.h>
#include <OpenCAX/Visualization/VtkViewer.h>

#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pnt.hxx>

using namespace OpenCAX;

int main()
{
    BRepBuilderAPI_MakePolygon poly;
    poly.Add(gp_Pnt(0, 0, 0));
    poly.Add(gp_Pnt(2, 0, 0));
    poly.Add(gp_Pnt(1, 1.5, 0));
    poly.Close();

    TopoDS_Shape shape = BRepBuilderAPI_MakeFace(poly.Wire()).Shape();

    MeshingOptions options;
    options.mesh_size_min = 0.05;
    options.mesh_size_max = 0.15;
    options.optimize = true;
    options.verbose = true;

    GmshMesher mesher;
    Mesh mesh = mesher.generateSurfaceMesh(shape, options);

    VtkViewer viewer;
    viewer.showMesh(mesh);

    return 0;
}
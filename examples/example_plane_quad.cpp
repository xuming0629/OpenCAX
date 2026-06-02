#include <OpenCAX/Meshing/GmshMesher.h>
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
    poly.Add(gp_Pnt(2, 1, 0));
    poly.Add(gp_Pnt(0, 1, 0));
    poly.Close();

    TopoDS_Shape shape = BRepBuilderAPI_MakeFace(poly.Wire()).Shape();

    MeshingOptions opt;
    opt.mesh_size_min = 0.1;
    opt.mesh_size_max = 0.1;
    opt.recombine = true;
    opt.transfinite = true;
    opt.nx = 20;
    opt.ny = 10;
    opt.verbose = true;

    Mesh mesh = GmshMesher::generateSurfaceMesh(shape, opt);

    VtkViewer viewer;
    viewer.showMesh(mesh);

    return 0;
}
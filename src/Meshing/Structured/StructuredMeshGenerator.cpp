#include <OpenCAX/Meshing/Structured/StructuredMeshGenerator.h>
#include <OpenCAX/Meshing/HighOrder/MeshOrderConverter.h>

namespace OpenCAX
{

TriangleMesh StructuredMeshGenerator::rectangle_tri3(double xmin,double xmax,double ymin,double ymax,int nx,int ny,bool alternate_diagonal)
{
    return TriangleMesh::create_structured_rectangle(xmin,xmax,ymin,ymax,nx,ny,alternate_diagonal);
}

TriangleMesh StructuredMeshGenerator::rectangle_tri6(double xmin,double xmax,double ymin,double ymax,int nx,int ny,bool alternate_diagonal)
{
    return MeshOrderConverter::triangle3_to_triangle6(rectangle_tri3(xmin,xmax,ymin,ymax,nx,ny,alternate_diagonal));
}

QuadMesh StructuredMeshGenerator::rectangle_quad4(double xmin,double xmax,double ymin,double ymax,int nx,int ny)
{
    return QuadMesh::create_structured_rectangle(xmin,xmax,ymin,ymax,nx,ny);
}

QuadMesh StructuredMeshGenerator::rectangle_quad8(double xmin,double xmax,double ymin,double ymax,int nx,int ny)
{
    return MeshOrderConverter::quad4_to_quad8(rectangle_quad4(xmin,xmax,ymin,ymax,nx,ny));
}

QuadMesh StructuredMeshGenerator::rectangle_quad9(double xmin,double xmax,double ymin,double ymax,int nx,int ny)
{
    return MeshOrderConverter::quad4_to_quad9(rectangle_quad4(xmin,xmax,ymin,ymax,nx,ny));
}

TetraMesh StructuredMeshGenerator::box_tetra4(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax,int nx,int ny,int nz)
{
    return TetraMesh::create_structured_box(xmin,xmax,ymin,ymax,zmin,zmax,nx,ny,nz);
}

TetraMesh StructuredMeshGenerator::box_tetra10(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax,int nx,int ny,int nz)
{
    return MeshOrderConverter::tetra4_to_tetra10(box_tetra4(xmin,xmax,ymin,ymax,zmin,zmax,nx,ny,nz));
}

HexMesh StructuredMeshGenerator::box_hexa8(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax,int nx,int ny,int nz)
{
    return HexMesh::create_structured_box(xmin,xmax,ymin,ymax,zmin,zmax,nx,ny,nz);
}

HexMesh StructuredMeshGenerator::box_hexa20(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax,int nx,int ny,int nz)
{
    return MeshOrderConverter::hexa8_to_hexa20(box_hexa8(xmin,xmax,ymin,ymax,zmin,zmax,nx,ny,nz));
}

HexMesh StructuredMeshGenerator::box_hexa27(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax,int nx,int ny,int nz)
{
    return MeshOrderConverter::hexa8_to_hexa27(box_hexa8(xmin,xmax,ymin,ymax,zmin,zmax,nx,ny,nz));
}

} // namespace OpenCAX

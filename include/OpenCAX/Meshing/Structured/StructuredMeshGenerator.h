#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/TriangleMesh.h>
#include <OpenCAX/Mesh2/QuadMesh.h>
#include <OpenCAX/Mesh2/TetraMesh.h>
#include <OpenCAX/Mesh2/HexMesh.h>

namespace OpenCAX
{

class OpenCAX_API StructuredMeshGenerator
{
public:
    static TriangleMesh rectangle_tri3(double xmin, double xmax, double ymin, double ymax, int nx, int ny, bool alternate_diagonal = false);
    static TriangleMesh rectangle_tri6(double xmin, double xmax, double ymin, double ymax, int nx, int ny, bool alternate_diagonal = false);

    static QuadMesh rectangle_quad4(double xmin, double xmax, double ymin, double ymax, int nx, int ny);
    static QuadMesh rectangle_quad8(double xmin, double xmax, double ymin, double ymax, int nx, int ny);
    static QuadMesh rectangle_quad9(double xmin, double xmax, double ymin, double ymax, int nx, int ny);

    static TetraMesh box_tetra4(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int nx, int ny, int nz);
    static TetraMesh box_tetra10(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int nx, int ny, int nz);

    static HexMesh box_hexa8(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int nx, int ny, int nz);
    static HexMesh box_hexa20(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int nx, int ny, int nz);
    static HexMesh box_hexa27(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int nx, int ny, int nz);
};

} // namespace OpenCAX

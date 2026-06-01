#pragma once

#include <OpenCAX/Meshing/Mesh.h>

namespace OpenCAX
{

struct Vec3
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class StructuredMeshGenerator
{
public:


    // 3D 空间线网格
    static Mesh create_line(
        const Vec3& p0,
        const Vec3& p1,
        int nx
    );

     // 3D 空间平面四边形网格
    static Mesh create_quad(
        const Vec3& p00,
        const Vec3& p10,
        const Vec3& p11,
        const Vec3& p01,
        int nx,
        int ny,
        double tolerance = 1.0e-10
    );

    // 3D 空间三角形网格
    static Mesh create_tri(
        const Vec3& p00,
        const Vec3& p10,
        const Vec3& p11,
        const Vec3& p01,
        int nx,
        int ny,
        double tolerance = 1.0e-10
    );

    // 3D 空间六面体网格
    static Mesh create_hex(
        const Vec3& p000,
        const Vec3& p100,
        const Vec3& p110,
        const Vec3& p010,
        const Vec3& p001,
        const Vec3& p101,
        const Vec3& p111,
        const Vec3& p011,
        int nx,
        int ny,
        int nz
    );

   
};

}
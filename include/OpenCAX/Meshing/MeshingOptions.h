#pragma once

namespace OpenCAX
{

struct MeshingOptions
{
    double mesh_size_min = 0.01;
    double mesh_size_max = 1.0;

    int nx = 10;
    int ny = 10;
    int nz = 10;

    bool second_order = false;
    bool optimize = true;
    bool verbose = false;
};

}
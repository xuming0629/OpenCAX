#pragma once

#include <OpenCAX/Mesh2/MeshTypes.h>
#include <string>

namespace OpenCAX
{

struct MeshGenerationOptions
{
    MeshDimension dimension = MeshDimension::Dim2;
    CellType cell_type = CellType::Triangle3;
    int order = 1;

    double element_size = 0.1;
    double min_size = 0.0;
    double max_size = 0.0;

    int nx = 10;
    int ny = 10;
    int nz = 10;

    bool alternate_diagonal = false;
    bool recombine = false;
    bool optimize = true;
    bool high_order_optimize = true;
    bool curved = false;

    int algorithm_2d = 6;
    int algorithm_3d = 1;

    bool verbose = false;
    std::string model_name = "OpenCAX_Generated_Mesh";
};

} // namespace OpenCAX

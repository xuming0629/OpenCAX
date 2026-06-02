#pragma once

#include <string>

namespace OpenCAX
{

struct MeshingOptions
{
    // =========================================
    // Global mesh size
    // =========================================

    double mesh_size_min = 0.01;
    double mesh_size_max = 1.0;

    // =========================================
    // Structured / transfinite divisions
    // =========================================

    int nx = 10;
    int ny = 10;
    int nz = 10;

    // =========================================
    // Element order
    // =========================================

    bool second_order = false;

    // =========================================
    // Optimization
    // =========================================

    bool optimize = true;

    // =========================================
    // Verbose
    // =========================================

    bool verbose = false;

    // =========================================
    // Recombine
    // 2D: triangle -> quad
    // 3D: tet-dominant -> hex-dominant attempt
    // =========================================

    bool recombine = false;

    // =========================================
    // Transfinite mesh
    // Useful for box-like regular shapes
    // =========================================

    bool transfinite = false;

    // =========================================
    // Temporary files
    // =========================================

    bool save_temp_brep = false;

    // =========================================
    // Debug output
    // =========================================

    bool save_msh = false;
    std::string msh_file = "opencax_mesh.msh";

    // =========================================
    // Gmsh algorithms
    // =========================================

    int algorithm_2d = 6;
    int algorithm_3d = 1;

    int smoothing_steps = 10;
};

} // namespace OpenCAX
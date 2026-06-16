#include <OpenCAX/Meshing/StructuredMeshGenerator.h>
#include <OpenCAX/Visualization/VtkViewer.h>
#include <OpenCAX/Visualization/VisualOptions.h>

#include <iostream>

int main()
{
    using namespace OpenCAX;

    // ==========================================
    // unit box top surface
    //
    // (0,0,1) -------- (1,0,1)
    //    |                 |
    //    |                 |
    // (0,1,1) -------- (1,1,1)
    // ==========================================

    Mesh mesh = StructuredMeshGenerator::create_quad(
        Vec3{0.0, 0.0, 1.0},  // p00
        Vec3{1.0, 0.0, 1.0},  // p10
        Vec3{1.0, 1.0, 1.0},  // p11
        Vec3{0.0, 1.0, 1.0},  // p01
        5,
        5
    );

    std::cout << "===== Unit Box Plane Mesh =====" << std::endl;
    std::cout << "Nodes : " << mesh.num_nodes() << std::endl;
    std::cout << "Cells : " << mesh.num_cells() << std::endl;

    // ==========================================
    // visualization options
    // ==========================================

    VisualOptions options;

    options.show_node_ids = true;
    options.show_cell_ids = true;

    options.show_points = true;

    options.wireframe = true;

    options.point_size = 8.0;
    options.line_width = 2.0;

    // mesh color
    options.mesh_color = {0.8, 0.85, 1.0};

    // ==========================================
    // show mesh
    // ==========================================

    VtkViewer::showMesh(mesh, options);

    return 0;
}
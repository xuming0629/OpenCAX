#include <OpenCAX/Mesh/StructuredMeshGenerator.h>
#include <OpenCAX/Post/VtkViewer.h>
#include <OpenCAX/Post/VisualOptions.h>

#include <iostream>

int main()
{
    using namespace OpenCAX;

    Mesh mesh = StructuredMeshGenerator::create_tri(
        Vec3{0.0, 0.0, 1.0},  // p00
        Vec3{1.0, 0.0, 1.0},  // p10
        Vec3{1.0, 1.0, 1.0},  // p11
        Vec3{0.0, 1.0, 1.0},  // p01
        5,
        5
    );

    std::cout << "===== Unit Box Top Plane Triangle Mesh =====" << std::endl;
    std::cout << "Nodes : " << mesh.num_nodes() << std::endl;
    std::cout << "Cells : " << mesh.num_cells() << std::endl;

    VisualOptions options;
    options.show_node_ids = true;
    options.show_cell_ids = true;
    options.show_points = true;
    options.wireframe = true;
    options.point_size = 5.0;
    options.line_width = 1.5;

    VtkViewer::showMesh(mesh, options);

    return 0;
}
#include <OpenCAX/Meshing/StructuredMeshGenerator.h>
#include <OpenCAX/Visualization/VtkViewer.h>
#include <OpenCAX/Visualization/VisualOptions.h>

#include <iostream>

int main()
{
    using namespace OpenCAX;

    Mesh mesh = StructuredMeshGenerator::create_tet(
        Vec3{0.0, 0.0, 0.0}, // p000
        Vec3{1.0, 0.0, 0.0}, // p100
        Vec3{1.0, 1.0, 0.0}, // p110
        Vec3{0.0, 1.0, 0.0}, // p010

        Vec3{0.0, 0.0, 1.0}, // p001
        Vec3{1.0, 0.0, 1.0}, // p101
        Vec3{1.0, 1.0, 1.0}, // p111
        Vec3{0.0, 1.0, 1.0}, // p011

        3,
        3,
        3
    );

    std::cout << "===== Unit Box Tetra Mesh =====" << std::endl;
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
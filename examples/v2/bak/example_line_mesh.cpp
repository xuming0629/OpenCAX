#include <OpenCAX/Meshing/StructuredMeshGenerator.h>
#include <OpenCAX/Visualization/VtkViewer.h>
#include <OpenCAX/Visualization/VisualOptions.h>

#include <iostream>

int main()
{
    using namespace OpenCAX;

    Mesh mesh = StructuredMeshGenerator::create_line(
        Vec3{0.0, 0.0, 0.0},
        Vec3{1.0, 1.0, 1.0},
        10
    );

    std::cout << "===== Space Line Mesh =====" << std::endl;
    std::cout << "Nodes: " << mesh.num_nodes() << std::endl;
    std::cout << "Cells: " << mesh.num_cells() << std::endl;

    VisualOptions options;
    options.show_node_ids = true;
    options.show_cell_ids = true;
    options.show_points = true;
    options.wireframe = true;
    options.point_size = 8.0;
    options.line_width = 2.0;

    VtkViewer::showMesh(mesh, options);

    return 0;
}
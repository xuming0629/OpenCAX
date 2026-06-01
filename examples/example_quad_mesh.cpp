#include <OpenCAX/Meshing/StructuredMeshGenerator.h>
#include <OpenCAX/Visualization/VtkViewer.h>

#include <iostream>

int main()
{
    using namespace OpenCAX;

    Mesh mesh = StructuredMeshGenerator::create_quad(
        Vec3{0.0, 0.0, 0.0},
        Vec3{2.0, 0.0, 0.5},
        Vec3{2.0, 1.0, 0.5},
        Vec3{0.0, 1.0, 0.0},
        20,
        10
    );

    std::cout << "Quad plane mesh\n";
    std::cout << "Nodes: " << mesh.num_nodes() << std::endl;
    std::cout << "Cells: " << mesh.num_cells() << std::endl;

    VtkViewer::showMesh(mesh);

    return 0;
}
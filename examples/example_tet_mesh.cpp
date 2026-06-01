#include <OpenCAX/Meshing/StructuredMeshGenerator.h>
#include <OpenCAX/Visualization/VtkViewer.h>

#include <iostream>

using namespace OpenCAX;

int main()
{
    Mesh mesh = StructuredMeshGenerator::create_hex_grid(
        0.0, 1.0,
        0.0, 1.0,
        0.0, 1.0,
        5, 5, 5
    );

    std::cout << "===== Hex Mesh =====" << std::endl;
    std::cout << "Nodes : " << mesh.num_nodes() << std::endl;
    std::cout << "Cells : " << mesh.num_cells() << std::endl;

    OpenCAX::VtkViewer::showMesh(mesh);

    return 0;
}
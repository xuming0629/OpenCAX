#include <OpenCAX/Meshing/TetgenMesher.h>
#include <OpenCAX/Meshing/MeshingOptions.h>
#include <OpenCAX/Visualization/VtkViewer.h>

#include <iostream>

int main()
{
    using namespace OpenCAX;

    MeshingOptions options;
    options.mesh_size_min = 0.05;
    options.tetgen_max_volume = 0.005;
    options.tetgen_quality = 1.414;
    options.verbose = true;

    Mesh mesh = TetgenMesher::createBox(
        1.0,
        1.0,
        1.0,
        options
    );

    std::cout << "TetGen box mesh" << std::endl;
    std::cout << "Nodes: " << mesh.num_nodes() << std::endl;
    std::cout << "Cells: " << mesh.num_cells() << std::endl;

    for (std::size_t i = 0; i < mesh.nodes().size() && i < 5; ++i) {
        const auto& p = mesh.nodes()[i];

        std::cout << "P" << i << ": "
                  << p.x << ", "
                  << p.y << ", "
                  << p.z << std::endl;
    }

    VtkViewer viewer;
    viewer.showMesh(mesh);



    return 0;
}
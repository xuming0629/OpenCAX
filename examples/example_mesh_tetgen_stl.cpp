#include <OpenCAX/Meshing/TetgenMesher.h>
#include <OpenCAX/Visualization/VtkViewer.h>
#include <OpenCAX/Visualization/VisualOptions.h>

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    using namespace OpenCAX;

    std::string filename = "../assets/five_spoke_wheel.stl";

    if (argc > 1) {
        filename = argv[1];
    }

    MeshingOptions options;
    options.mesh_size_min = 0.05;
    options.tetgen_quality = 1.414;
    options.tetgen_max_volume = 0.001;
    options.verbose = true;

    Mesh mesh = TetgenMesher::generateVolumeMeshFromFile(
        filename,
        options
    );

    if (mesh.empty()) {
        std::cerr << "Failed to generate tetra mesh from: "
                  << filename << std::endl;
        return 1;
    }

    std::cout << "TetGen STL tetra mesh" << std::endl;
    std::cout << "File: " << filename << std::endl;
    std::cout << "Nodes: " << mesh.num_nodes() << std::endl;
    std::cout << "Tetra cells: " << mesh.num_cells() << std::endl;

    for (std::size_t i = 0; i < mesh.nodes().size() && i < 5; ++i) {
        const auto& p = mesh.nodes()[i];

        std::cout << "Node " << i << ": "
                  << p.x << ", "
                  << p.y << ", "
                  << p.z << std::endl;
    }

    for (std::size_t i = 0; i < mesh.cells().size() && i < 5; ++i) {
        const auto& c = mesh.cells()[i];

        std::cout << "Cell " << i << ": ";
        for (int id : c.node_ids) {
            std::cout << id << " ";
        }
        std::cout << std::endl;
    }
    VtkViewer viewer;
    viewer.showMesh(mesh);


    return 0;
}
#include <OpenCAX/Meshing/TetgenMesher.h>
#include <OpenCAX/Visualization/VtkViewer.h>
#include <OpenCAX/IO/VtkIO.h>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    using namespace OpenCAX;

    std::string filename = "../assets/fan.stl";

    if (argc > 1) {
        filename = argv[1];
    }

    MeshingOptions options;

    options.mesh_size_min = 0.05;
    options.mesh_size_max = 1.0;

    // TetGen 质量参数
    options.tetgen_quality = 1.414;

    // 先不要太小，否则四面体数量会暴涨
    options.tetgen_max_volume = 1.0;

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
    std::cout << "Cells: " << mesh.num_cells() << std::endl;

    for (std::size_t i = 0; i < mesh.nodes().size() && i < 5; ++i) {
        const auto& p = mesh.nodes()[i];

        std::cout << "Node " << i << ": "
                  << p.x << ", "
                  << p.y << ", "
                  << p.z << std::endl;
    }

    std::vector<std::vector<int>> cell_node_ids;
    std::vector<CellType> cell_types;

    cell_node_ids.reserve(mesh.cells().size());
    cell_types.reserve(mesh.cells().size());

    for (const auto& cell : mesh.cells()) {
        cell_node_ids.push_back(cell.node_ids);

        if (cell.type != CellType::Unknown) {
            cell_types.push_back(cell.type);
        }
        else if (cell.node_ids.size() == 4) {
            cell_types.push_back(CellType::Tetra4);
        }
        else if (cell.node_ids.size() == 3) {
            cell_types.push_back(CellType::Triangle3);
        }
        else if (cell.node_ids.size() == 2) {
            cell_types.push_back(CellType::Line2);
        }
        else if (cell.node_ids.size() == 1) {
            cell_types.push_back(CellType::Point1);
        }
        else {
            cell_types.push_back(CellType::Unknown);
        }
    }

    for (std::size_t i = 0; i < cell_node_ids.size() && i < 5; ++i) {
        std::cout << "Cell " << i << " [type="
                  << static_cast<int>(cell_types[i])
                  << "]: ";

        for (int id : cell_node_ids[i]) {
            std::cout << id << " ";
        }

        std::cout << std::endl;
    }

    const std::string out_vtu = "tetgen_result.vtu";

    const bool saved = VtkIO::writeVTU(
        mesh.nodes(),
        cell_node_ids,
        cell_types,
        out_vtu,
        true
    );

    if (!saved) {
        std::cerr << "Failed to save VTU file: "
                  << out_vtu << std::endl;
        return 1;
    }

    std::cout << "Saved: " << out_vtu << std::endl;

    VtkViewer viewer;
    viewer.showMesh(mesh);

    return 0;
}
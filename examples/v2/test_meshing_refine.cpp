#include <OpenCAX/Meshing/Structured/StructuredMeshGenerator.h>
#include <OpenCAX/Meshing/Refinement/MeshRefiner.h>
#include <OpenCAX/Meshing/Quality/MeshQuality.h>
#include <OpenCAX/Post/Viewer/VisualOptions.h>
#include <OpenCAX/Post/Viewer/VtkViewer.h>
#include <iostream>

int main()
{
    auto mesh = OpenCAX::StructuredMeshGenerator::rectangle_tri3(
        0.0, 1.0,
        0.0, 1.0,
        1, 1
    );

    auto refined = OpenCAX::MeshRefiner::uniform_refine(mesh, 2);
    auto quality = OpenCAX::MeshQuality::summarize(refined);

    std::cout << "original cells = " << mesh.num_cells() << "\n";
    std::cout << "refined cells  = " << refined.num_cells() << "\n";
    std::cout << "min area       = " << quality.min_measure << "\n";


    OpenCAX::VisualOptions option;

    option.show_node_ids = true;
    option.show_cell_ids = true;
    option.show_edge_ids = false;

    option.show_nodes = true;
    option.show_edges = true;
    option.show_axes = true;

    option.window_title = "Triangle3 Mesh uniform refine";

    OpenCAX::VtkViewer::showMesh(
        refined,
        option
    );

    return 0;
}

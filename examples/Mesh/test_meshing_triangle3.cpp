#include <OpenCAX/Meshing/Structured/StructuredMeshGenerator.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Mesh/MeshDebugUtils.h>
#include <OpenCAX/Post/Viewer/VisualOptions.h>
#include <OpenCAX/Post/Viewer/VtkViewer.h>


#include <iostream>
#include <string>

int main()
{
    auto mesh = OpenCAX::StructuredMeshGenerator::rectangle_tri3(
        0.0, 1.0,
        0.0, 1.0,
        2, 2
    );

    std::string error;
    if (!mesh.validate(&error))
    {
        std::cerr << error << "\n";
        return 1;
    }

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    std::cout << "nodes = " << mesh.num_nodes() << "\n";
    std::cout << "cells = " << mesh.num_cells() << "\n";
    std::cout << "area  = " << mesh.total_area() << "\n";
    std::cout << "topology edges = " << topo.num_edges() << "\n";

    OpenCAX::MeshDebugUtils::print_topology(topo);

    OpenCAX::VisualOptions option;

    option.show_node_ids = true;
    option.show_cell_ids = true;
    option.show_edge_ids = true;

    option.show_nodes = true;
    option.show_edges = true;
    option.show_axes = true;

    option.window_title = "Quadratic Triangle3 Mesh";

    OpenCAX::VtkViewer::showMesh(
        mesh,
        option
    );

    // return 0;
}

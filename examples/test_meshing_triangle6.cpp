#include <OpenCAX/Meshing/Structured/StructuredMeshGenerator.h>
#include <OpenCAX/Mesh2/MeshTopology.h>
#include <OpenCAX/Mesh2/MeshDebugUtils.h>
#include <OpenCAX/Post/Viewer/VisualOptions.h>
#include <OpenCAX/Post/Viewer/VtkViewer.h>
#include <OpenCAX/Core/Logger.h>
#include <OpenCAX/Core/LogMacros.h>


#include <iostream>
#include <string>

int main()
{   
    // OpenCAX::Logger::init(
    //     OpenCAX::LogLevel::Info,
    //     "OpenCAX_mesh.log"
    // );

    auto mesh = OpenCAX::StructuredMeshGenerator::rectangle_tri6(
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
    OpenCAX::MeshDebugUtils::print_mesh(mesh);
    OpenCAX_MESH_INFO("nodes num = {}", mesh.num_nodes());
    OpenCAX_MESH_INFO("cells num = {}", mesh.num_cells());
    OpenCAX_MESH_INFO("area  = {}", mesh.total_area());
    OpenCAX_MESH_INFO("topology edges = {}", topo.num_edges());

    OpenCAX::MeshDebugUtils::print_topology(topo);

    OpenCAX::VisualOptions option;

    option.show_node_ids = true;
    option.show_cell_ids = true;
    option.show_edge_ids = false;

    option.show_nodes = true;
    option.show_edges = true;
    option.show_axes = true;

    option.window_title = "Quadratic Triangle6 Mesh";

    OpenCAX::VtkViewer::showMesh(
        mesh,
        option
    );

    // return 0;
}

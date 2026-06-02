#include <OpenCAX/Core/Logger.h>
#include <OpenCAX/Core/LogMacros.h>

#include <OpenCAX/Meshing/GmshMesher.h>
#include <OpenCAX/Meshing/MeshingOptions.h>

#include <BRepPrimAPI_MakeBox.hxx>

#include <iostream>
#include <OpenCAX/Visualization/VtkViewer.h>
#include <OpenCAX/Visualization/VisualOptions.h>

int main()
{
    using namespace OpenCAX;
    OpenCAX::Logger::init(OpenCAX::LogLevel::Debug);

    TopoDS_Shape box = BRepPrimAPI_MakeBox(
        1.0,
        1.0,
        1.0
    ).Shape();

    OpenCAX::MeshingOptions options;
    options.mesh_size_min = 0.25;
    options.mesh_size_max = 0.5;
    options.second_order = false;
    options.optimize = true;
    options.verbose = false;

    OpenCAX::Mesh mesh =
        OpenCAX::GmshMesher::generateVolumeMesh(
            box,
            options
        );

    std::cout << "Gmsh volume mesh\n";
    std::cout << "===== Unit Box Tetra Mesh =====" << std::endl;
    std::cout << "Nodes : " << mesh.num_nodes() << std::endl;
    std::cout << "Cells : " << mesh.num_cells() << std::endl;

    OpenCAX::VisualOptions visual_options;
    visual_options.show_node_ids = true;
    visual_options.show_cell_ids = true;
    visual_options.show_points = true;
    visual_options.wireframe = true;
    visual_options.point_size = 5.0;
    visual_options.line_width = 1.5;

    VtkViewer::showMesh(mesh, visual_options);

    return 0;
}
// int main()
// {
//     auto shape = OpenCAX::ShapeFactory::makeBox(1.0, 1.0, 1.0);

//     OpenCAX::OccIO::writeSTEP(shape, "box.step");
//     OpenCAX::OccIO::writeBREP(shape, "box.brep");

//     OpenCAX::MeshingOptions options;
//     options.mesh_size_min = 0.1;
//     options.mesh_size_max = 0.1;
//     options.optimize = true;
//     options.verbose = true;

//     auto mesh = OpenCAX::GmshMesher::generateVolumeMesh(shape, options);

//     std::cout << "nodes: " << mesh.nodes().size() << std::endl;
//     std::cout << "cells: " << mesh.cells().size() << std::endl;

//     OpenCAX::VtkViewer::showMesh(mesh);

//     return 0;
// }
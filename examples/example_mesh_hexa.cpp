#include <OpenCAX/Core/Logger.h>
#include <OpenCAX/Core/LogMacros.h>

#include <OpenCAX/Meshing/GmshMesher.h>
#include <OpenCAX/Meshing/MeshingOptions.h>

#include <OpenCAX/Visualization/VtkViewer.h>
#include <OpenCAX/Visualization/VisualOptions.h>

#include <BRepPrimAPI_MakeBox.hxx>

#include <iostream>

int main()
{
    using namespace OpenCAX;

    Logger::init(LogLevel::Debug);

    TopoDS_Shape box =
        BRepPrimAPI_MakeBox(
            1.0,
            1.0,
            1.0
        ).Shape();

    MeshingOptions options;

    options.nx = 4;
    options.ny = 4;
    options.nz = 4;

    options.second_order = false;
    options.optimize = false;
    options.verbose = true;

    // true: 尝试生成 Hexa8
    options.recombine = true;
    options.transfinite = true;

    Mesh mesh =
        GmshMesher::generateVolumeMesh(
            box,
            options
        );

    std::cout << "===== Gmsh Hex Mesh =====" << std::endl;
    std::cout << "Nodes : " << mesh.num_nodes() << std::endl;
    std::cout << "Cells : " << mesh.num_cells() << std::endl;

    VisualOptions visual_options;
    visual_options.show_node_ids = true;
    visual_options.show_cell_ids = true;
    visual_options.show_points = true;
    visual_options.wireframe = true;
    visual_options.point_size = 5.0;
    visual_options.line_width = 1.5;

    VtkViewer::showMesh(
        mesh,
        visual_options
    );

    Logger::shutdown();

    return 0;
}
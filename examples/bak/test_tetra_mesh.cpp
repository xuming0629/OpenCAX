#include <OpenCAX/Post/VtkViewer.h>
#include <OpenCAX/Mesh/TetraMesh.h>


#include <iostream>

int main()
{
    using namespace OpenCAX;

    OpenCAX::TetraMesh mesh =
    OpenCAX::TetraMesh::create_structured_box(
        0.0, 1.0,
        0.0, 1.0,
        0.0, 1.0,
        1, 1, 1
    );

    // std::cout << "nodes = " << mesh.num_nodes() << std::endl;
    // std::cout << "cells = " << mesh.num_cells() << std::endl;
    // std::cout << "tetras = " << mesh.num_tetras() << std::endl;
    // std::cout << "edges = " << mesh.edges().size() << std::endl;
    // std::cout << "faces = " << mesh.faces().size() << std::endl;
    // std::cout << "boundary faces = " << mesh.boundary_faces().size() << std::endl;
    // std::cout << "boundary nodes = " << mesh.boundary_nodes().size() << std::endl;
    // std::cout << "volume = " << mesh.total_volume() << std::endl;

    // VisualOptions options;
    // options.show_node_ids = true;
    // options.show_cell_ids = true;
    // options.show_points = true;
    // options.wireframe = true;
    // options.point_size = 5.0;
    // options.line_width = 1.5;

    // VtkViewer::showMesh(tet_mesh, options);

    return 0;
}
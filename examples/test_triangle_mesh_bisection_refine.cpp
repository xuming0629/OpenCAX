#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/TriangleMeshRefiner.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Mesh/MeshDebugUtils.h>
#include <OpenCAX/Post/VtkViewer.h>

#include <iostream>
#include <string>

int main()
{
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0,
            1.0,
            0.0,
            1.0,
            1,
            1
        );

    std::cout << "===== Original Mesh =====\n";
    std::cout << "nodes = " << mesh.num_nodes() << "\n";
    std::cout << "cells = " << mesh.num_cells() << "\n";
    std::cout << "area  = " << mesh.total_area() << "\n";

    OpenCAX::TriangleMesh refined =
        OpenCAX::TriangleMeshRefiner::bisection_refine(
            mesh,
            1
        );

    std::cout << "\n===== Bisection Refined Mesh =====\n";
    std::cout << "nodes = " << refined.num_nodes() << "\n";
    std::cout << "cells = " << refined.num_cells() << "\n";
    std::cout << "area  = " << refined.total_area() << "\n";

    std::string error;

    if (!refined.validate(&error))
    {
        std::cerr << "invalid refined mesh: "
                  << error
                  << "\n";
        return 1;
    }

    OpenCAX::MeshTopology topo;
    topo.build(refined);

    OpenCAX::MeshDebugUtils::print_mesh(refined);
    OpenCAX::MeshDebugUtils::check_mesh(refined);
    OpenCAX::MeshDebugUtils::print_topology(refined, topo);

    OpenCAX::VtkViewer::showMeshWithIds(refined);

    return 0;
}
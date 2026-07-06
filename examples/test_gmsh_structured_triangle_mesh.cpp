#include <OpenCAX/Mesh/GmshTriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Post/VtkViewer.h>
#include <OpenCAX/Mesh/MeshDebugUtils.h>

#include <iostream>
#include <string>

int main()
{
    OpenCAX::GmshTriangleMeshOptions options;

    options.mesh_size = 0.1;
    options.optimize = false;
    options.verbose = true;
    options.algorithm = 6;
    options.model_name = "gmsh_structured_rectangle";

    const int nx = 4;
    const int ny = 4;

    OpenCAX::TriangleMesh mesh =
        OpenCAX::GmshTriangleMeshGenerator::create_structured_rectangle(
            0.0,
            1.0,
            0.0,
            1.0,
            nx,
            ny,
            options
        );

   

    std::string error;

    if (!mesh.validate(&error))
    {
        std::cerr << "invalid triangle mesh: "
                  << error << "\n";
        OpenCAX::GmshTriangleMeshGenerator::finalize();
        return 1;
    }

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    OpenCAX::MeshDebugUtils::print_mesh(mesh);
    OpenCAX::MeshDebugUtils::check_mesh(mesh);
    OpenCAX::MeshDebugUtils::print_topology(mesh, topo);

    OpenCAX::VtkViewer::showMeshWithIds(mesh);

    OpenCAX::GmshTriangleMeshGenerator::finalize();

    return 0;
}
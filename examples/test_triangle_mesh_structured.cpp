#include <OpenCAX/Mesh/TriangleMesh.h>
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
            2,
            2
        );

    std::string error;

    if (!mesh.validate(&error))
    {
        std::cerr << "invalid triangle mesh: "
                  << error
                  << "\n";
        return 1;
    }

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    OpenCAX::MeshDebugUtils::print_mesh(mesh);
    OpenCAX::MeshDebugUtils::check_mesh(mesh);
    OpenCAX::MeshDebugUtils::print_topology(mesh, topo);

    OpenCAX::VtkViewer::showMeshWithIds(mesh);

    return 0;
}
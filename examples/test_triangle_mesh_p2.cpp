#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/TriangleMeshOrderConverter.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Mesh/MeshDebugUtils.h>

#include <OpenCAX/Post/Viewer/VtkViewer.h>

#include <iostream>
#include <string>

int main()
{
    /*
     * 1. 创建一阶 Triangle3 网格
     */
    OpenCAX::TriangleMesh linear_mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0,
            1.0,
            0.0,
            1.0,
            2,
            2
        );

    std::string error;

    if (!linear_mesh.validate(&error))
    {
        std::cerr
            << "invalid linear triangle mesh: "
            << error
            << "\n";

        return 1;
    }

    std::cout
        << "===== Linear Triangle3 Mesh =====\n";

    std::cout
        << "nodes = "
        << linear_mesh.num_nodes()
        << "\n";

    std::cout
        << "cells = "
        << linear_mesh.num_cells()
        << "\n";

    std::cout
        << "area  = "
        << linear_mesh.total_area()
        << "\n";

    /*
     * 2. 转换成二阶 Triangle6 网格
     */
    OpenCAX::TriangleMesh quadratic_mesh =
        OpenCAX::TriangleMeshOrderConverter::to_quadratic(
            linear_mesh
        );

    if (!quadratic_mesh.validate(&error))
    {
        std::cerr
            << "invalid quadratic triangle mesh: "
            << error
            << "\n";

        return 1;
    }

    std::cout
        << "\n===== Quadratic Triangle6 Mesh =====\n";

    std::cout
        << "nodes = "
        << quadratic_mesh.num_nodes()
        << "\n";

    std::cout
        << "cells = "
        << quadratic_mesh.num_cells()
        << "\n";

    /*
     * 对 2x2 结构三角形网格：
     *
     * Triangle3:
     *   nodes = 9
     *   cells = 8
     *   edges = 16
     *
     * Triangle6:
     *   nodes = 9 + 16 = 25
     *   cells = 8
     */
    OpenCAX::MeshTopology topo;
    topo.build(quadratic_mesh);

    std::cout
        << "edges = "
        << topo.num_edges()
        << "\n";

    std::cout
        << "boundary edges = "
        << topo.boundary_edges().size()
        << "\n";

    std::cout
        << "boundary nodes = "
        << topo.boundary_nodes().size()
        << "\n";

    OpenCAX::MeshDebugUtils::print_mesh(
        quadratic_mesh
    );

    OpenCAX::MeshDebugUtils::check_mesh(
        quadratic_mesh
    );

    OpenCAX::MeshDebugUtils::print_topology(
        quadratic_mesh,
        topo
    );

    /*
     * 3. 显示二阶网格
     */
    OpenCAX::VtkViewer::showMeshWithIds(
        quadratic_mesh
    );

    return 0;
}
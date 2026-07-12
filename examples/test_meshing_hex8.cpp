#include <OpenCAX/Meshing/Structured/StructuredMeshGenerator.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Mesh/MeshDebugUtils.h>
#include <OpenCAX/Post/Viewer/VisualOptions.h>
#include <OpenCAX/Post/Viewer/VtkViewer.h>

#include <iostream>
#include <string>

int main()
{
    // 创建 [0, 1] × [0, 1] × [0, 1] 的结构化八节点六面体网格
    //
    // 每个方向划分为：
    // nx = 2
    // ny = 2
    // nz = 2
    //
    // 因此：
    // 节点数 = (nx + 1)(ny + 1)(nz + 1) = 27
    // 单元数 = nx * ny * nz = 8
    auto mesh = OpenCAX::StructuredMeshGenerator::box_hexa8(
        0.0, 1.0,     // xmin, xmax
        0.0, 1.0,     // ymin, ymax
        0.0, 1.0,     // zmin, zmax
        2, 2, 2       // nx, ny, nz
    );

    std::string error;
    if (!mesh.validate(&error))
    {
        std::cerr << "Invalid hexahedral mesh: "
                  << error << "\n";
        return 1;
    }

    // 构建六面体网格拓扑关系
    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    std::cout << "===== HEXAHEDRAL MESH =====\n";

    std::cout << "nodes          = "
              << mesh.num_nodes() << "\n";

    std::cout << "hexa cells     = "
              << mesh.num_cells() << "\n";

    std::cout << "total volume   = "
              << mesh.total_volume() << "\n";

    std::cout << "topology edges = "
              << topo.num_edges() << "\n";

    // 如果 MeshTopology 已经支持三维面拓扑，可以增加：
    //
    // std::cout << "topology faces = "
    //           << topo.num_faces() << "\n";
    //
    // std::cout << "boundary faces = "
    //           << topo.boundary_faces().size() << "\n";

    // 打印完整拓扑信息
    OpenCAX::MeshDebugUtils::print_topology(topo);

    // VTK 显示选项
    OpenCAX::VisualOptions option;

    option.show_node_ids = true;
    option.show_cell_ids = true;
    option.show_edge_ids = false;

    option.show_nodes = true;
    option.show_edges = true;
    option.show_axes = true;

    option.window_title = "Structured Hexa8 Mesh";

    OpenCAX::VtkViewer::showMesh(
        mesh,
        option
    );

    return 0;
}
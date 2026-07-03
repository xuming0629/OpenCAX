/**
 * @file test_triangle_mesh_unstructured.cpp
 * @brief 测试 TriangleMesh 非结构三角形网格构造
 *
 * 本示例测试：
 *
 * - TriangleMesh::create_unstructured()
 * - 非结构三角形网格节点和单元构造
 * - 面积计算
 * - 质心计算
 * - MeshTopology 拓扑构建
 * - 边界边、内部边、相邻单元
 */

#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <array>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace OpenCAX;

int main()
{
    std::cout << "===== OpenCAX TriangleMesh Unstructured Test =====" << std::endl;

    /*
     * 创建一个正方形区域的非结构三角网格。
     *
     * 几何区域：
     *
     * 3 ----- 2
     * |     / |
     * |   /   |
     * | /     |
     * 0 ----- 1
     *
     * 两个三角形：
     *
     * cell 0: 0, 1, 2
     * cell 1: 0, 2, 3
     *
     * 理论结果：
     *
     * - nodes = 4
     * - cells = 2
     * - edges = 5
     * - boundary_edges = 4
     * - internal_edges = 1
     * - total_area = 1
     */
    std::vector<std::array<double, 3>> points = {
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {1.0, 1.0, 0.0},
        {0.0, 1.0, 0.0}
    };

    std::vector<std::array<int, 3>> triangles = {
        {0, 1, 2},
        {0, 2, 3}
    };

    TriangleMesh mesh = TriangleMesh::create_unstructured(
        points,
        triangles
    );

    std::cout << "mesh name        = " << mesh.info().name << std::endl;
    std::cout << "mesh source      = " << mesh.info().source << std::endl;
    std::cout << "num nodes        = " << mesh.num_nodes() << std::endl;
    std::cout << "num cells        = " << mesh.num_cells() << std::endl;
    std::cout << "is unstructured  = " << std::boolalpha << mesh.is_unstructured() << std::endl;

    std::cout << std::fixed << std::setprecision(6);

    for (std::size_t i = 0; i < mesh.num_cells(); ++i)
    {
        const int cell_id = static_cast<int>(i);

        const double a = mesh.area(cell_id);
        const auto c = mesh.centroid(cell_id);

        std::cout << "cell " << cell_id << " area     = " << a << std::endl;
        std::cout << "cell " << cell_id << " centroid = "
                  << "(" << c[0] << ", " << c[1] << ", " << c[2] << ")"
                  << std::endl;
    }

    std::cout << "total area       = " << mesh.total_area() << std::endl;

    /*
     * 构建拓扑关系。
     */
    MeshTopology topology;
    topology.build(mesh);

    std::cout << "topology built   = " << topology.built() << std::endl;
    std::cout << "num edges        = " << topology.num_edges() << std::endl;
    std::cout << "num faces        = " << topology.num_faces() << std::endl;

    std::cout << "boundary edges   = " << topology.boundary_edges().size() << std::endl;
    std::cout << "boundary nodes   = " << topology.boundary_nodes().size() << std::endl;
    std::cout << "boundary cells   = " << topology.boundary_cells().size() << std::endl;

    std::cout << "cell2cell:" << std::endl;

    for (std::size_t i = 0; i < topology.cell2cell().size(); ++i)
    {
        std::cout << "  cell " << i << " neighbors:";

        for (int neighbor_id : topology.cell2cell()[i])
        {
            std::cout << " " << neighbor_id;
        }

        std::cout << std::endl;
    }

    std::cout << "edge2cell:" << std::endl;

    for (std::size_t i = 0; i < topology.edge2cell().size(); ++i)
    {
        const auto& e = topology.edges()[i];

        std::cout << "  edge " << i
                  << " (" << e[0] << ", " << e[1] << ") cells:";

        for (int cell_id : topology.edge2cell()[i])
        {
            std::cout << " " << cell_id;
        }

        std::cout << std::endl;
    }

    /*
     * 简单正确性检查。
     */
    if (!mesh.validate())
    {
        std::cerr << "[FAILED] mesh validate failed." << std::endl;
        return 1;
    }

    if (mesh.num_nodes() != 4)
    {
        std::cerr << "[FAILED] expected 4 nodes." << std::endl;
        return 1;
    }

    if (mesh.num_cells() != 2)
    {
        std::cerr << "[FAILED] expected 2 cells." << std::endl;
        return 1;
    }

    if (topology.num_edges() != 5)
    {
        std::cerr << "[FAILED] expected 5 edges." << std::endl;
        return 1;
    }

    if (topology.boundary_edges().size() != 4)
    {
        std::cerr << "[FAILED] expected 4 boundary edges." << std::endl;
        return 1;
    }

    if (topology.cell2cell().size() != 2)
    {
        std::cerr << "[FAILED] expected 2 cell2cell entries." << std::endl;
        return 1;
    }

    if (topology.cell2cell()[0].size() != 1 || topology.cell2cell()[0][0] != 1)
    {
        std::cerr << "[FAILED] expected cell 0 neighbor is cell 1." << std::endl;
        return 1;
    }

    if (topology.cell2cell()[1].size() != 1 || topology.cell2cell()[1][0] != 0)
    {
        std::cerr << "[FAILED] expected cell 1 neighbor is cell 0." << std::endl;
        return 1;
    }

    std::cout << "[PASSED] TriangleMesh unstructured test passed." << std::endl;

    return 0;
}
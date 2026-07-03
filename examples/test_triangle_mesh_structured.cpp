#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>
// #include <OpenCAX/Mesh/MeshModule.h>
// #include <OpenCAX/Post/MeshPlotter.h>
#include <OpenCAX/Post/VtkViewer.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace OpenCAX;

/**
 * @brief 计算三角形在 XY 平面上的有向面积 2 倍
 *
 * 大于 0 表示逆时针。
 * 小于 0 表示顺时针。
 */
static double signed_area2_xy(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    return (b.x - a.x) * (c.y - a.y) -
           (b.y - a.y) * (c.x - a.x);
}

/**
 * @brief 判断 node_ids[1] 是否为直角点
 *
 * 对三角形 {a, b, c}，如果 b 是直角点，则：
 *
 * 向量 ba = a - b
 * 向量 bc = c - b
 *
 * 应满足：
 *
 * ba · bc = 0
 */
static bool right_angle_at_middle(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c,
    double eps = 1.0e-12
)
{
    const double bax = a.x - b.x;
    const double bay = a.y - b.y;
    const double baz = a.z - b.z;

    const double bcx = c.x - b.x;
    const double bcy = c.y - b.y;
    const double bcz = c.z - b.z;

    const double dot = bax * bcx + bay * bcy + baz * bcz;

    return std::abs(dot) <= eps;
}

/**
 * @brief 打印一个 vector<int>
 */
static void print_vector_int(
    const std::vector<int>& values
)
{
    std::cout << "[";

    for (std::size_t i = 0; i < values.size(); ++i)
    {
        std::cout << values[i];

        if (i + 1 < values.size())
        {
            std::cout << ", ";
        }
    }

    std::cout << "]";
}

/**
 * @brief 检查三角形方向和直角点
 */
static bool check_triangle_orientation_and_right_angle(
    const TriangleMesh& mesh
)
{
    bool ok = true;

    const auto& nodes = mesh.nodes();
    const auto& cells = mesh.cells();

    std::cout << "\n===== Check Triangle Orientation =====" << std::endl;

    for (const auto& cell : cells)
    {
        if (cell.type != CellType::Triangle3)
        {
            std::cerr << "[FAILED] cell " << cell.id
                      << " is not Triangle3." << std::endl;
            ok = false;
            continue;
        }

        if (cell.node_ids.size() != 3)
        {
            std::cerr << "[FAILED] cell " << cell.id
                      << " does not have 3 nodes." << std::endl;
            ok = false;
            continue;
        }

        const int n0 = cell.node_ids[0];
        const int n1 = cell.node_ids[1];
        const int n2 = cell.node_ids[2];

        const MeshNode& a = nodes[static_cast<std::size_t>(n0)];
        const MeshNode& b = nodes[static_cast<std::size_t>(n1)];
        const MeshNode& c = nodes[static_cast<std::size_t>(n2)];

        const double area2 = signed_area2_xy(a, b, c);
        const bool ccw = area2 > 0.0;
        const bool right_mid = right_angle_at_middle(a, b, c);

        std::cout << "cell " << cell.id
                  << " node_ids=";
        print_vector_int(cell.node_ids);

        std::cout << " signed_area2=" << std::setw(10) << area2
                  << " ccw=" << std::boolalpha << ccw
                  << " right_at_middle=" << right_mid
                  << std::endl;

        if (!ccw)
        {
            std::cerr << "[FAILED] cell " << cell.id
                      << " is not counter-clockwise." << std::endl;
            ok = false;
        }

        if (!right_mid)
        {
            std::cerr << "[FAILED] cell " << cell.id
                      << " right angle is not at node_ids[1]." << std::endl;
            ok = false;
        }
    }

    return ok;
}

/**
 * @brief 打印节点
 */
static void print_nodes(
    const TriangleMesh& mesh
)
{
    std::cout << "\n===== Nodes =====" << std::endl;

    for (const auto& node : mesh.nodes())
    {
        std::cout << "node " << node.id
                  << " = ("
                  << node.x << ", "
                  << node.y << ", "
                  << node.z << ")"
                  << std::endl;
    }
}

/**
 * @brief 打印单元
 */
static void print_cells(
    const TriangleMesh& mesh
)
{
    std::cout << "\n===== Cells =====" << std::endl;

    for (const auto& cell : mesh.cells())
    {
        std::cout << "cell " << cell.id
                  << " node_ids=";
        print_vector_int(cell.node_ids);
        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "===== OpenCAX TriangleMesh Orientation Test =====" << std::endl;

    /*
     * 生成 2 x 2 结构三角形网格。
     *
     * 预期：
     *
     * - nodes = 9
     * - cells = 8
     * - edges = 16
     * - boundary_edges = 8
     *
     * 每个三角形：
     *
     * - 必须逆时针
     * - 直角点必须在 node_ids[1]
     */
    TriangleMesh mesh = TriangleMesh::create_structured_rectangle(
        0.0,
        1.0,
        0.0,
        1.0,
        4,
        4,
        false
    );

    std::cout << "mesh name     = " << mesh.info().name << std::endl;
    std::cout << "mesh source   = " << mesh.info().source << std::endl;
    std::cout << "num nodes     = " << mesh.num_nodes() << std::endl;
    std::cout << "num cells     = " << mesh.num_cells() << std::endl;
    std::cout << "is structured = " << std::boolalpha << mesh.is_structured() << std::endl;
    
    print_nodes(mesh);
    print_cells(mesh);


    VisualOptions options;
    options.show_node_ids = true;
    options.show_cell_ids = true;
    options.show_points = true;
    options.wireframe = true;
    options.point_size = 5.0;
    options.line_width = 1.5;

    VtkViewer::showMesh(mesh, options);
    /*
     * 检查拓扑。
     */
    // MeshTopology topology;
    // topology.build(mesh);

    // std::cout << "\n===== Topology =====" << std::endl;
    // std::cout << "topology built = " << topology.built() << std::endl;
    // std::cout << "num edges      = " << topology.num_edges() << std::endl;
    // std::cout << "num nodes"     = " << topology.num_nodes() << std::endl;

    // std::cout << "boundary edges = " << topology.boundary_edges().size() << std::endl;
    // std::cout << "boundary nodes = " << topology.boundary_nodes().size() << std::endl;
    // std::cout << "boundary cells = " << topology.boundary_cells().size() << std::endl;

   

    return 0;
}
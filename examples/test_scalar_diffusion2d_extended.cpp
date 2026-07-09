#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <OpenCAX/FEM/P1TriangleSpace.h>
#include <OpenCAX/FEM/ScalarDiffusion2D.h>
#include <OpenCAX/FEM/CGSolver.h>

#include <OpenCAX/Post/ScalarFieldViewer.h>

#include <Eigen/Dense>

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

static double gaussian_source(
    double x,
    double y
)
{
    const double cx = 0.5;
    const double cy = 0.5;
    const double sigma = 0.08;

    const double dx = x - cx;
    const double dy = y - cy;

    const double r2 = dx * dx + dy * dy;

    return 500.0 * std::exp(
        -r2 / (2.0 * sigma * sigma)
    );
}

static double material_k(
    double x,
    double y
)
{
    const double cx = 0.5;
    const double cy = 0.5;
    const double r = 0.18;

    const double dx = x - cx;
    const double dy = y - cy;

    const double dist2 = dx * dx + dy * dy;

    /*
     * 中心圆形高导热材料
     */
    if (dist2 < r * r)
    {
        return 30.0;
    }

    /*
     * 左右两种材料
     */
    if (x < 0.5)
    {
        return 10.0;
    }

    return 1.0;
}

static void print_solution_statistics(
    const OpenCAX::TriangleMesh& mesh,
    const Eigen::VectorXd& u
)
{
    double u_min =
        std::numeric_limits<double>::max();

    double u_max =
        -std::numeric_limits<double>::max();

    double u_sum = 0.0;

    int min_id = -1;
    int max_id = -1;

    for (int i = 0; i < u.size(); ++i)
    {
        const double value = u(i);

        u_sum += value;

        if (value < u_min)
        {
            u_min = value;
            min_id = i;
        }

        if (value > u_max)
        {
            u_max = value;
            max_id = i;
        }
    }

    const double u_avg =
        u_sum / static_cast<double>(u.size());

    std::cout << "\n===== Solution Statistics =====\n";
    std::cout << "min u = " << u_min
              << " at node " << min_id << "\n";
    std::cout << "max u = " << u_max
              << " at node " << max_id << "\n";
    std::cout << "avg u = " << u_avg << "\n";

    if (min_id >= 0)
    {
        const auto& p =
            mesh.nodes()[static_cast<std::size_t>(min_id)];

        std::cout << "min point = ("
                  << p.x << ", "
                  << p.y << ", "
                  << p.z << ")\n";
    }

    if (max_id >= 0)
    {
        const auto& p =
            mesh.nodes()[static_cast<std::size_t>(max_id)];

        std::cout << "max point = ("
                  << p.x << ", "
                  << p.y << ", "
                  << p.z << ")\n";
    }
}

static int nearest_node(
    const OpenCAX::TriangleMesh& mesh,
    double x,
    double y
)
{
    int nearest = -1;

    double best =
        std::numeric_limits<double>::max();

    const auto& nodes = mesh.nodes();

    for (std::size_t i = 0; i < nodes.size(); ++i)
    {
        const double dx = nodes[i].x - x;
        const double dy = nodes[i].y - y;

        const double d2 = dx * dx + dy * dy;

        if (d2 < best)
        {
            best = d2;
            nearest = static_cast<int>(i);
        }
    }

    return nearest;
}

static void print_probe_values(
    const OpenCAX::TriangleMesh& mesh,
    const Eigen::VectorXd& u
)
{
    std::vector<std::pair<double, double>> probes =
    {
        {0.25, 0.50},
        {0.50, 0.50},
        {0.75, 0.50},
        {0.50, 0.80}
    };

    std::cout << "\n===== Probe Values =====\n";

    for (const auto& p : probes)
    {
        const int node_id =
            nearest_node(
                mesh,
                p.first,
                p.second
            );

        if (node_id < 0)
        {
            continue;
        }

        const auto& node =
            mesh.nodes()[static_cast<std::size_t>(node_id)];

        std::cout << "target = ("
                  << p.first << ", "
                  << p.second << "), nearest node = "
                  << node_id
                  << ", coord = ("
                  << node.x << ", "
                  << node.y << "), u = "
                  << u(node_id)
                  << "\n";
    }
}

int main()
{
    /*
     * 1. 创建二维三角形网格
     */
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0,
            1.0,
            0.0,
            1.0,
            80,
            80
        );

    std::string error;

    if (!mesh.validate(&error))
    {
        std::cerr
            << "invalid mesh: "
            << error
            << std::endl;

        return 1;
    }

    std::cout << "===== Mesh Info =====\n";
    std::cout << "nodes = "
              << mesh.num_nodes()
              << "\n";
    std::cout << "cells = "
              << mesh.num_cells()
              << "\n";
    std::cout << "area  = "
              << mesh.total_area()
              << "\n";

    /*
     * 2. 构建拓扑
     */
    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    std::cout << "edges = "
              << topo.num_edges()
              << "\n";
    std::cout << "boundary edges = "
              << topo.boundary_edges().size()
              << "\n";
    std::cout << "boundary nodes = "
              << topo.boundary_nodes().size()
              << "\n";

    /*
     * 3. 构建 P1 有限元空间
     */
    OpenCAX::P1TriangleSpace V(
        mesh,
        topo
    );

    /*
     * 4. 构建标量扩散问题
     *
     *    -div(k grad u) = f
     */
    OpenCAX::ScalarDiffusion2D problem(V);

    /*
     * 5. 设置空间变化材料 k(x,y)
     */
    problem.setCoefficient(
        [](double x, double y)
        {
            return material_k(x, y);
        }
    );

    /*
     * 6. 设置内部源项 f(x,y)
     */
    problem.setSource(
        [](double x, double y)
        {
            return gaussian_source(x, y);
        }
    );

    const double eps = 1.0e-12;

    /*
     * 7. 左边界 Dirichlet: u = 100
     */
    problem.addDirichletBC(
        OpenCAX::BoundarySelector2D::byPredicate(
            [eps](double x, double)
            {
                return std::abs(x - 0.0) < eps;
            }
        ),
        [](double, double)
        {
            return 100.0;
        }
    );

    /*
     * 8. 右边界 Dirichlet: u = 0
     */
    problem.addDirichletBC(
        OpenCAX::BoundarySelector2D::byPredicate(
            [eps](double x, double)
            {
                return std::abs(x - 1.0) < eps;
            }
        ),
        [](double, double)
        {
            return 0.0;
        }
    );

    /*
     * 9. 上边界局部 Neumann 热流:
     *
     *    只在 x ∈ [0.35, 0.65], y = 1 上施加 q = 20
     *
     *    注意：
     *    对于 -div(k grad u)=f，
     *    Neumann 通常表示 k grad u · n = q。
     */
    problem.addNeumannBC(
        OpenCAX::BoundarySelector2D::byPredicate(
            [eps](double x, double y)
            {
                return
                    std::abs(y - 1.0) < eps &&
                    x >= 0.35 &&
                    x <= 0.65;
            }
        ),
        [](double, double)
        {
            return 20.0;
        }
    );

    /*
     * 10. 下边界绝热 q = 0
     *
     *     不写也等价，因为自然边界默认就是 0。
     *     这里保留，方便你测试 Neumann 接口。
     */
    problem.addNeumannBC(
        OpenCAX::BoundarySelector2D::byPredicate(
            [eps](double, double y)
            {
                return std::abs(y - 0.0) < eps;
            }
        ),
        [](double, double)
        {
            return 0.0;
        }
    );

    /*
     * 11. 设置求解器
     */
    auto solver =
        std::make_shared<OpenCAX::CGSolver>();

    solver->setTolerance(1.0e-12);
    solver->setMaxIterations(100000);

    problem.setSolver(solver);

    /*
     * 12. 求解
     */
    if (!problem.solve())
    {
        std::cerr
            << "solve failed"
            << std::endl;

        return 1;
    }

    /*
     * 13. 输出统计量
     */
    print_solution_statistics(
        mesh,
        problem.solution()
    );

    /*
     * 14. 输出监测点结果
     */
    print_probe_values(
        mesh,
        problem.solution()
    );

    /*
     * 15. 显示结果
     */
    OpenCAX::ScalarFieldViewer::showSolution2D(
        mesh,
        problem.solution(),
        "Scalar Diffusion: multi-material + source + mixed BC"
    );

    return 0;
}
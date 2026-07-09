#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <OpenCAX/FEM/P1TriangleSpace.h>
#include <OpenCAX/FEM/ScalarDiffusion2D.h>
#include <OpenCAX/FEM/CGSolver.h>
#include <OpenCAX/Post/Recovery/FieldRecovery2D.h>

#include <OpenCAX/Post/Viewer/ScalarFieldViewer.h>
#include <OpenCAX/Post/Viewer/VectorFieldViewer.h>

#include <Eigen/Dense>

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief 多材料导热系数 k(x,y)
 *
 * 中心圆形区域：k = 30
 * 左半区域：k = 10
 * 右半区域：k = 1
 */
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

    const double dist2 =
        dx * dx + dy * dy;

    if (dist2 < r * r)
    {
        return 30.0;
    }

    if (x < 0.5)
    {
        return 10.0;
    }

    return 1.0;
}

/**
 * @brief 内部高斯热源 f(x,y)
 */
static double source_f(
    double x,
    double y
)
{
    const double cx = 0.5;
    const double cy = 0.5;
    const double sigma = 0.08;

    const double dx = x - cx;
    const double dy = y - cy;

    const double r2 =
        dx * dx + dy * dy;

    return 500.0 * std::exp(
        -r2 / (2.0 * sigma * sigma)
    );
}

/**
 * @brief 输出标量场的统计信息
 */
static void print_scalar_statistics(
    const OpenCAX::TriangleMesh& mesh,
    const Eigen::VectorXd& values,
    const std::string& name
)
{
    if (values.size() == 0)
    {
        std::cout
            << "[Statistics] "
            << name
            << " is empty.\n";

        return;
    }

    double v_min =
        std::numeric_limits<double>::max();

    double v_max =
        -std::numeric_limits<double>::max();

    double v_sum = 0.0;

    int min_id = -1;
    int max_id = -1;

    for (int i = 0; i < values.size(); ++i)
    {
        const double v =
            values(i);

        v_sum += v;

        if (v < v_min)
        {
            v_min = v;
            min_id = i;
        }

        if (v > v_max)
        {
            v_max = v;
            max_id = i;
        }
    }

    const double v_avg =
        v_sum / static_cast<double>(values.size());

    std::cout
        << "\n===== Statistics: "
        << name
        << " =====\n";

    std::cout
        << "min = "
        << v_min
        << ", node = "
        << min_id
        << "\n";

    std::cout
        << "max = "
        << v_max
        << ", node = "
        << max_id
        << "\n";

    std::cout
        << "avg = "
        << v_avg
        << "\n";

    if (min_id >= 0)
    {
        const auto& p =
            mesh.nodes()[static_cast<std::size_t>(min_id)];

        std::cout
            << "min point = ("
            << p.x
            << ", "
            << p.y
            << ", "
            << p.z
            << ")\n";
    }

    if (max_id >= 0)
    {
        const auto& p =
            mesh.nodes()[static_cast<std::size_t>(max_id)];

        std::cout
            << "max point = ("
            << p.x
            << ", "
            << p.y
            << ", "
            << p.z
            << ")\n";
    }
}

/**
 * @brief 找离指定坐标最近的节点
 */
static int nearest_node(
    const OpenCAX::TriangleMesh& mesh,
    double x,
    double y
)
{
    int nearest = -1;

    double best =
        std::numeric_limits<double>::max();

    const auto& nodes =
        mesh.nodes();

    for (std::size_t i = 0; i < nodes.size(); ++i)
    {
        const double dx =
            nodes[i].x - x;

        const double dy =
            nodes[i].y - y;

        const double d2 =
            dx * dx + dy * dy;

        if (d2 < best)
        {
            best = d2;
            nearest =
                static_cast<int>(i);
        }
    }

    return nearest;
}

/**
 * @brief 输出若干监测点数值
 */
static void print_probe_values(
    const OpenCAX::TriangleMesh& mesh,
    const Eigen::VectorXd& values,
    const std::string& name
)
{
    std::vector<std::pair<double, double>> probes =
    {
        {0.25, 0.50},
        {0.50, 0.50},
        {0.75, 0.50},
        {0.50, 0.80},
        {0.50, 0.20}
    };

    std::cout
        << "\n===== Probe Values: "
        << name
        << " =====\n";

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

        std::cout
            << "target = ("
            << p.first
            << ", "
            << p.second
            << "), nearest node = "
            << node_id
            << ", coord = ("
            << node.x
            << ", "
            << node.y
            << "), "
            << name
            << " = "
            << values(node_id)
            << "\n";
    }
}

int main()
{
    std::cout
        << "========================================\n"
        << " OpenCAX FEM Example\n"
        << " Scalar Diffusion / Heat Conduction 2D\n"
        << "========================================\n";

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

    std::cout
        << "\n===== Mesh Info =====\n";

    std::cout
        << "nodes = "
        << mesh.num_nodes()
        << "\n";

    std::cout
        << "cells = "
        << mesh.num_cells()
        << "\n";

    std::cout
        << "area  = "
        << mesh.total_area()
        << "\n";

    /*
     * 2. 构建拓扑
     */
    OpenCAX::MeshTopology topo;
    topo.build(mesh);

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

    /*
     * 3. 构建 P1 有限元空间
     */
    OpenCAX::P1TriangleSpace V(
        mesh,
        topo
    );

    std::cout
        << "\n===== Function Space =====\n";

    std::cout
        << "space = "
        << V.name()
        << "\n";

    std::cout
        << "dofs  = "
        << V.numDofs()
        << "\n";

    /*
     * 4. 构建标量扩散问题
     *
     *    -div(k grad u) = f
     */
    OpenCAX::ScalarDiffusion2D problem(V);

    /*
     * 5. 设置材料 k(x,y)
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
            return source_f(x, y);
        }
    );

    const double eps =
        1.0e-12;

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
     * 9. 顶部局部 Neumann 热流
     *
     *    在 y = 1, x ∈ [0.35, 0.65] 上施加：
     *
     *        k grad u · n = 20
     *
     *    对于 -div(k grad u) = f，
     *    Neumann 边界项进入右端项。
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
     * 10. 底部边界绝热
     *
     * q = 0 是自然边界，不写也等价。
     * 这里写出来用于演示 Neumann 接口。
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

    solver->setTolerance(
        1.0e-12
    );

    solver->setMaxIterations(
        100000
    );

    problem.setSolver(
        solver
    );

    /*
     * 12. 求解
     */
    std::cout
        << "\n===== Solve =====\n";

    if (!problem.solve())
    {
        std::cerr
            << "solve failed"
            << std::endl;

        return 1;
    }

    const Eigen::VectorXd& u =
        problem.solution();

    /*
     * 13. 后处理：计算 |grad u|
     */
    Eigen::VectorXd grad_mag =
        OpenCAX::FieldRecovery2D::recoverNodalGradientMagnitude(
            mesh,
            u
        );

    /*
     * 14. 后处理：计算 qx
     */
    Eigen::VectorXd flux_x =
        OpenCAX::FieldRecovery2D::recoverNodalFluxX(
            mesh,
            u,
            [](double x, double y)
            {
                return material_k(x, y);
            }
        );

    /*
     * 15. 后处理：计算 qy
     */
    Eigen::VectorXd flux_y =
        OpenCAX::FieldRecovery2D::recoverNodalFluxY(
            mesh,
            u,
            [](double x, double y)
            {
                return material_k(x, y);
            }
        );

    /*
     * 16. 后处理：计算 |q|
     *
     * q = -k grad u
     */
    Eigen::VectorXd flux_mag =
        OpenCAX::FieldRecovery2D::recoverNodalFluxMagnitude(
            mesh,
            u,
            [](double x, double y)
            {
                return material_k(x, y);
            }
        );

    /*
     * 17. 输出统计信息
     */
    print_scalar_statistics(
        mesh,
        u,
        "u"
    );

    print_scalar_statistics(
        mesh,
        grad_mag,
        "|grad u|"
    );

    print_scalar_statistics(
        mesh,
        flux_mag,
        "|q|"
    );

    /*
     * 18. 输出监测点结果
     */
    print_probe_values(
        mesh,
        u,
        "u"
    );

    print_probe_values(
        mesh,
        flux_mag,
        "|q|"
    );

    /*
     * 19. 可视化
     *
     * 注意：
     * 每个 showSolution2D 都会打开一个 VTK 窗口。
     * 关闭当前窗口后，才会显示下一个。
     */
    /*
    * 19. 可视化
    *
    * 注意：
    * 每个窗口关闭后，才会打开下一个。
    */
    OpenCAX::ScalarFieldViewer::showSolution2D(
        mesh,
        u,
        "u: temperature / scalar potential"
    );

    OpenCAX::ScalarFieldViewer::showSolution2D(
        mesh,
        grad_mag,
        "|grad u|"
    );

    OpenCAX::ScalarFieldViewer::showSolution2D(
        mesh,
        flux_mag,
        "|q| = |-k grad u|"
    );

    OpenCAX::ScalarFieldViewer::showSolution2D(
        mesh,
        flux_x,
        "qx"
    );

    OpenCAX::ScalarFieldViewer::showSolution2D(
        mesh,
        flux_y,
        "qy"
    );

    OpenCAX::VectorFieldViewer::showVectorField2DWithMagnitude(
        mesh,
        flux_x,
        flux_y,
        "flux vector q = -k grad u",
        0.06
    );

    return 0;
}
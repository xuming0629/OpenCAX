#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <OpenCAX/FEM/P1TriangleSpace.h>
#include <OpenCAX/FEM/HeatEquation2D.h>
#include <OpenCAX/FEM/CGSolver.h>

#include <OpenCAX/Post/ScalarFieldViewer.h>

#include <cmath>
#include <iostream>
#include <memory>

int main()
{
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(0, 1, 0, 1, 40, 40);

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    OpenCAX::P1TriangleSpace V(mesh, topo);
    OpenCAX::HeatEquation2D heat(V);

    heat.setCapacity(1.0);
    heat.setConductivity(1.0);
    heat.setSource([](double, double) { return 0.0; });

    // 初值：中间高温包
    heat.setInitialCondition([](double x, double y) {
        const double dx = x - 0.5;
        const double dy = y - 0.5;
        return 100.0 * std::exp(-80.0 * (dx * dx + dy * dy));
    });

    const double eps = 1.0e-12;

    // 四周固定温度 0
    heat.addDirichletBC(
        OpenCAX::BoundarySelector2D::byPredicate([eps](double x, double y) {
            return std::abs(x) < eps || std::abs(x - 1.0) < eps ||
                   std::abs(y) < eps || std::abs(y - 1.0) < eps;
        }),
        [](double, double) { return 0.0; }
    );

    auto solver = std::make_shared<OpenCAX::CGSolver>();
    solver->setTolerance(1.0e-12);
    solver->setMaxIterations(50000);
    heat.setSolver(solver);

    heat.initialize();

    if (!heat.run(0.001, 100))
    {
        std::cerr << "time marching failed" << std::endl;
        return 1;
    }

    OpenCAX::ScalarFieldViewer::showSolution2D(mesh, heat.solution(), "HeatEquation2D t=0.1");

    return 0;
}

#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <OpenCAX/FEM/P1TriangleSpace.h>
#include <OpenCAX/FEM/ScalarDiffusion2D.h>
#include <OpenCAX/FEM/CGSolver.h>

#include <OpenCAX/Post/ScalarFieldViewer.h>

#include <cmath>
#include <iostream>
#include <memory>

int main()
{
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0, 1.0,
            0.0, 1.0,
            50, 50
        );

    std::string error;
    if (!mesh.validate(&error))
    {
        std::cerr << error << std::endl;
        return 1;
    }

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    OpenCAX::P1TriangleSpace V(mesh, topo);
    OpenCAX::ScalarDiffusion2D problem(V);

    // k(x,y): 左半区域材料导热/扩散能力高，右半区域低
    problem.setCoefficient([](double x, double) {
        return x < 0.5 ? 10.0 : 1.0;
    });

    problem.setSource([](double, double) {
        return 0.0;
    });

    const double eps = 1.0e-12;

    // 左边界 u=100
    problem.addDirichletBC(
        OpenCAX::BoundarySelector2D::byPredicate([eps](double x, double) {
            return std::abs(x - 0.0) < eps;
        }),
        [](double, double) { return 100.0; }
    );

    // 右边界 u=0
    problem.addDirichletBC(
        OpenCAX::BoundarySelector2D::byPredicate([eps](double x, double) {
            return std::abs(x - 1.0) < eps;
        }),
        [](double, double) { return 0.0; }
    );

    // 上下边界自然 Neumann=0，不写也等价。这里演示接口。
    problem.addNeumannBC(
        OpenCAX::BoundarySelector2D::byPredicate([eps](double, double y) {
            return std::abs(y - 0.0) < eps || std::abs(y - 1.0) < eps;
        }),
        [](double, double) { return 0.0; }
    );

    auto solver = std::make_shared<OpenCAX::CGSolver>();
    solver->setTolerance(1.0e-12);
    solver->setMaxIterations(50000);
    problem.setSolver(solver);

    if (!problem.solve())
    {
        std::cerr << "solve failed" << std::endl;
        return 1;
    }

    OpenCAX::ScalarFieldViewer::showSolution2D(
        mesh,
        problem.solution(),
        "Scalar Diffusion: material + BC"
    );

    return 0;
}

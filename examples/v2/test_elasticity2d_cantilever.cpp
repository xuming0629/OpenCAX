#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <OpenCAX/FEM/P1TriangleSpace.h>
#include <OpenCAX/FEM/Elasticity2D.h>
#include <OpenCAX/FEM/SparseLUSolver.h>

#include <iostream>
#include <memory>
#include <cmath>

int main()
{
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0, 4.0,
            0.0, 1.0,
            80, 20
        );

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    OpenCAX::P1TriangleSpace V(mesh, topo);
    OpenCAX::Elasticity2D elas(V);

    elas.setMaterial(OpenCAX::LinearElasticMaterial2D(
        2.1e11,
        0.30,
        OpenCAX::ElasticModel2D::PlaneStress
    ));

    elas.setBodyForce([](double, double) {
        return Eigen::Vector2d(0.0, 0.0);
    });

    const double eps = 1.0e-12;

    // 左边界固支：u=v=0
    elas.addDirichletBC(
        OpenCAX::BoundarySelector2D::byPredicate([eps](double x, double) {
            return std::abs(x - 0.0) < eps;
        }),
        [](double, double) {
            return Eigen::Vector2d(0.0, 0.0);
        }
    );

    // 右端向下均布牵引
    elas.addTractionBC(
        OpenCAX::BoundarySelector2D::byPredicate([eps](double x, double) {
            return std::abs(x - 4.0) < eps;
        }),
        [](double, double) {
            return Eigen::Vector2d(0.0, -1.0e5);
        }
    );

    elas.setSolver(std::make_shared<OpenCAX::SparseLUSolver>());

    if (!elas.solve())
    {
        std::cerr << "elasticity solve failed" << std::endl;
        return 1;
    }

    std::cout << "solution dofs = " << elas.displacement().size() << std::endl;

    // 打印右上角附近最大位移
    double umax = 0.0;
    for (int i = 0; i < static_cast<int>(mesh.num_nodes()); ++i)
    {
        const auto u = elas.nodeDisplacement(i);
        umax = std::max(umax, u.norm());
    }

    std::cout << "max displacement = " << umax << std::endl;

    return 0;
}

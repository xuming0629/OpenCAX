#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/FEM/Poisson2D.h>
#include <OpenCAX/FEM/SparseLUSolver.h>

#include <iostream>
#include <memory>

int main()
{
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0, 1.0,
            0.0, 1.0,
            30, 30
        );

    OpenCAX::Poisson2D poisson(mesh);

    poisson.setSource([](double, double) {
        return 1.0;
    });

    poisson.setDirichlet([](double, double) {
        return 0.0;
    });

    poisson.setSolver(std::make_shared<OpenCAX::SparseLUSolver>());

    if (!poisson.solve())
    {
        return 1;
    }

    poisson.showSolution("Poisson2D P1 SparseLU");

    return 0;
}

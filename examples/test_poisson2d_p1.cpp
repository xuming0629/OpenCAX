#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Mesh/MeshDebugUtils.h>

#include <OpenCAX/FEM/P1TriangleSpace.h>
#include <OpenCAX/FEM/Poisson2D.h>
#include <OpenCAX/FEM/CGSolver.h>

#include <OpenCAX/Post/Viewer/ScalarFieldViewer.h>

#include <iostream>
#include <memory>
#include <string>

int main()
{
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0,
            1.0,
            0.0,
            1.0,
            20,
            20
        );

    std::string error;

    if (!mesh.validate(&error))
    {
        std::cerr << "invalid triangle mesh: "
                  << error
                  << "\n";
        return 1;
    }

    OpenCAX::MeshTopology topo;
    topo.build(mesh);

    OpenCAX::MeshDebugUtils::print_mesh(mesh);
    OpenCAX::MeshDebugUtils::check_mesh(mesh);
    OpenCAX::MeshDebugUtils::print_topology(mesh, topo);

    OpenCAX::P1TriangleSpace V(
        mesh,
        topo
    );

    OpenCAX::Poisson2D poisson(V);

    poisson.setSource(
        [](double, double) {
            return 1.0;
        }
    );

    poisson.setDirichlet(
        [](double, double) {
            return 0.0;
        }
    );

    auto solver =
        std::make_shared<OpenCAX::CGSolver>();

    solver->setTolerance(1.0e-12);
    solver->setMaxIterations(10000);

    poisson.setSolver(solver);

    if (!poisson.solve())
    {
        std::cerr << "Poisson solve failed.\n";
        return 1;
    }

    OpenCAX::ScalarFieldViewer::showSolution2D(
        mesh,
        poisson.solution(),
        "Poisson2D P1 CG"
    );

    return 0;
}
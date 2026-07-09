#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <OpenCAX/FEM/P1TriangleSpace.h>
#include <OpenCAX/FEM/Poisson2D.h>
#include <OpenCAX/FEM/CGSolver.h>
#include <OpenCAX/FEM/ConvergenceStudy2D.h>

#include <OpenCAX/Post/Viewer/ScalarFieldViewer.h>
#include <OpenCAX/Post/Plot/ConvergencePlotter.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    constexpr double pi =
        3.141592653589793238462643383279502884;

    auto exact = [](double x, double y) {
        return std::sin(pi * x) * std::sin(pi * y);
    };

    auto grad_exact = [](double x, double y) {
        return std::array<double, 2>{
            pi * std::cos(pi * x) * std::sin(pi * y),
            pi * std::sin(pi * x) * std::cos(pi * y)
        };
    };

    auto source = [](double x, double y) {
        return 2.0 * pi * pi *
               std::sin(pi * x) *
               std::sin(pi * y);
    };

    std::vector<OpenCAX::ConvergenceRecord2D> records;

    const int base_n = 4;
    const int levels = 5;

    for (int level = 0; level < levels; ++level)
    {
        const int n =
            base_n * (1 << level);

        OpenCAX::TriangleMesh mesh =
            OpenCAX::TriangleMesh::create_structured_rectangle(
                0.0,
                1.0,
                0.0,
                1.0,
                n,
                n
            );

        std::string error;

        if (!mesh.validate(&error))
        {
            std::cerr << "invalid mesh: "
                      << error
                      << std::endl;
            return 1;
        }

        OpenCAX::MeshTopology topo;
        topo.build(mesh);

        OpenCAX::P1TriangleSpace V(
            mesh,
            topo
        );

        OpenCAX::Poisson2D poisson(V);

        poisson.setSource(source);

        poisson.setDirichlet(
            [](double, double) {
                return 0.0;
            }
        );

        auto solver =
            std::make_shared<OpenCAX::CGSolver>();

        solver->setTolerance(1.0e-13);
        solver->setMaxIterations(20000);

        poisson.setSolver(solver);

        if (!poisson.solve())
        {
            std::cerr << "solve failed at level "
                      << level
                      << std::endl;
            return 1;
        }

        auto record =
            OpenCAX::ConvergenceStudy2D::computeRecord(
                level,
                mesh,
                poisson.solution(),
                exact,
                grad_exact
            );

        records.push_back(record);

        std::cout << "level = "
                  << level
                  << ", n = "
                  << n
                  << ", h = "
                  << record.h
                  << ", L2 = "
                  << record.L2
                  << ", H1 = "
                  << record.H1
                  << std::endl;

        if (level == levels - 1)
        {
            OpenCAX::ScalarFieldViewer::showSolution2D(
                mesh,
                poisson.solution(),
                "Poisson2D Numerical Solution"
            );
        }
    }

    OpenCAX::ConvergenceStudy2D::computeOrders(records);

    OpenCAX::ConvergenceStudy2D::print(records);

    OpenCAX::ConvergenceStudy2D::writeCSV(
        records,
        "poisson2d_convergence.csv"
    );

    OpenCAX::ConvergencePlotter::writePythonScript(
        "poisson2d_convergence.csv",
        "plot_poisson2d_convergence.py",
        "poisson2d_convergence.png"
    );

    std::cout << "CSV saved to poisson2d_convergence.csv\n";
    std::cout << "Python plot script saved to plot_poisson2d_convergence.py\n";
    std::cout << "Run:\n";
    std::cout << "  python plot_poisson2d_convergence.py\n";

    return 0;
}
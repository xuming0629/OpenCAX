#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/TriangleMeshRefiner.h>
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


    /*
     * 精确解
     *
     * u = sin(pi*x)sin(pi*y)
     *
     * -Delta u = f
     *
     */
    auto exact =
        [](double x, double y)
    {
        return std::sin(pi*x)
             * std::sin(pi*y);
    };


    /*
     * 精确梯度
     */
    auto grad_exact =
        [](double x, double y)
    {
        return std::array<double,2>
        {
            pi * std::cos(pi*x)
              * std::sin(pi*y),

            pi * std::sin(pi*x)
              * std::cos(pi*y)
        };
    };


    /*
     * Poisson source
     *
     * -Delta u=f
     */
    auto source =
        [](double x, double y)
    {
        return
            2.0*pi*pi
            *
            std::sin(pi*x)
            *
            std::sin(pi*y);
    };



    std::vector<
        OpenCAX::ConvergenceRecord2D
    > records;



    /*
     * 初始粗网格
     *
     * 后面通过 uniform_refine
     * 进行一致加密
     */
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0,
            1.0,
            0.0,
            1.0,
            2,
            2
        );


    const int levels = 5;


    for(int level=0; level<levels; ++level)
    {

        std::cout
            << "\n====================\n"
            << "level = "
            << level
            << "\n";


        std::cout
            << "nodes = "
            << mesh.num_nodes()
            << "\n";


        std::cout
            << "cells = "
            << mesh.num_cells()
            << "\n";


        std::cout
            << "area = "
            << mesh.total_area()
            << "\n";



        std::string error;


        if(!mesh.validate(&error))
        {
            std::cerr
                << "invalid mesh:"
                << error
                << std::endl;

            return 1;
        }



        /*
         * topology
         */
        OpenCAX::MeshTopology topo;

        topo.build(mesh);



        /*
         * FEM space
         */
        OpenCAX::P1TriangleSpace V(
            mesh,
            topo
        );



        /*
         * Poisson
         */
        OpenCAX::Poisson2D poisson(V);



        poisson.setSource(source);



        poisson.setDirichlet(
            [](double,double)
            {
                return 0.0;
            }
        );



        auto solver =
            std::make_shared<
                OpenCAX::CGSolver
            >();


        solver->setTolerance(
            1e-13
        );


        solver->setMaxIterations(
            20000
        );


        poisson.setSolver(
            solver
        );



        if(!poisson.solve())
        {
            std::cerr
                <<"solve failed\n";

            return 1;
        }



        /*
         * 计算误差
         */
        auto record =
            OpenCAX::ConvergenceStudy2D::computeRecord(
                level,
                mesh,
                poisson.solution(),
                exact,
                grad_exact
            );


        records.push_back(record);



        std::cout
            << "L2 = "
            << record.L2
            << "\n";


        std::cout
            << "H1 = "
            << record.H1
            << "\n";



        /*
         * 最后一层显示解
         */
        if(level == levels-1)
        {

            OpenCAX::ScalarFieldViewer::
                showSolution2D(
                    mesh,
                    poisson.solution(),
                    "Poisson2D Uniform Refinement"
                );
        }



        /*
         * 下一层一致加密
         *
         * 最后一层不用继续
         */
        if(level != levels-1)
        {

            mesh =
                OpenCAX::TriangleMeshRefiner::
                uniform_refine(
                    mesh,
                    1
                );
        }

    }



    /*
     * 计算收敛阶
     */
    OpenCAX::ConvergenceStudy2D::
        computeOrders(records);



    OpenCAX::ConvergenceStudy2D::
        print(records);



    OpenCAX::ConvergenceStudy2D::
        writeCSV(
            records,
            "poisson2d_uniform_refine.csv"
        );



    OpenCAX::ConvergencePlotter::
        writePythonScript(
            "poisson2d_uniform_refine.csv",
            "plot_uniform_refine.py",
            "uniform_refine.png"
        );


    std::cout
        << "\nDone.\n";


    return 0;
}
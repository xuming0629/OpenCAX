// #include <OpenCAX/Mesh/TriangleMesh.h>
// #include <OpenCAX/FEM/Poisson2D.h>

// #include <iostream>
// #include <algorithm>

// int main()
// {
//     OpenCAX::TriangleMesh mesh =
//         OpenCAX::TriangleMesh::create_structured_rectangle(
//             0.0, 1.0,
//             0.0, 1.0,
//             40, 40
//         );

//     OpenCAX::Poisson2D poisson(mesh);

//     poisson.setSource([](double, double) {
//         return 1.0;
//     });

//     poisson.setDirichlet([](double, double) {
//         return 0.0;
//     });

//     if (!poisson.solve())
//     {
//         return 1;
//     }

//     const auto& u = poisson.solution();

//     std::cout << "Poisson2D solved." << std::endl;
//     std::cout << "nodes = " << mesh.num_nodes() << std::endl;
//     std::cout << "triangles = " << mesh.num_triangles() << std::endl;
//     std::cout << "solution size = " << u.size() << std::endl;

//     double umin = u.minCoeff();
//     double umax = u.maxCoeff();

//     std::cout << "u min = " << umin << std::endl;
//     std::cout << "u max = " << umax << std::endl;

//     for (int i = 0; i < std::min<int>(10, u.size()); ++i)
//     {
//         std::cout << "u[" << i << "] = " << u[i] << std::endl;
//     }

//     return 0;
// }

#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/FEM/Poisson2D.h>
#include <OpenCAX/Post/ScalarFieldViewer.h>

#include <iostream>

int main()
{
    OpenCAX::TriangleMesh mesh =
        OpenCAX::TriangleMesh::create_structured_rectangle(
            0.0, 1.0,
            0.0, 1.0,
            60, 60
        );

    OpenCAX::Poisson2D poisson(mesh);

    poisson.setSource([](double, double) {
        return 1.0;
    });

    poisson.setDirichlet([](double, double) {
        return 0.0;
    });

    if (!poisson.solve())
    {
        return 1;
    }

    const auto& u = poisson.solution();

    std::cout << "u min = " << u.minCoeff() << std::endl;
    std::cout << "u max = " << u.maxCoeff() << std::endl;

    OpenCAX::ScalarFieldViewer::showSolution(
        mesh,
        u,
        "Poisson2D Solution"
    );

    return 0;
}
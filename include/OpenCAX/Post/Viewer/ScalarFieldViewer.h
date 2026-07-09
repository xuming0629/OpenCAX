#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/TriangleMesh.h>

#include <Eigen/Dense>

namespace OpenCAX
{

class OpenCAX_API ScalarFieldViewer
{
public:
    static void showSolution2D(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& values,
        const char* title = "Scalar Field"
    );

    static void showSolution(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& values,
        const char* title = "Scalar Field"
    );
};

} // namespace OpenCAX
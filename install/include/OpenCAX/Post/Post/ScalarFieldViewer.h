#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <Eigen/Dense>

namespace OpenCAX
{

class OpenCAX_API ScalarFieldViewer
{
public:
    static void showSolution(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& u,
        const char* title = "OpenCAX Scalar Field"
    );
};

} // namespace OpenCAX
#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <Eigen/Dense>

namespace OpenCAX
{

class OpenCAX_API ScalarFieldViewer
{
public:
    /**
     * @brief 显示二维三角形网格上的节点标量场
     *
     * @param mesh 三角形网格
     * @param values 节点标量值，大小应等于 mesh.num_nodes()
     * @param title 窗口标题
     */
    static void showSolution2D(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& values,
        const char* title = "Scalar Field"
    );

    /**
     * @brief 兼容旧名字
     */
    static void showSolution(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& values,
        const char* title = "Scalar Field"
    );
};

} // namespace OpenCAX
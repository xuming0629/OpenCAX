#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/TriangleMesh.h>

#include <Eigen/Dense>

namespace OpenCAX
{

class OpenCAX_API VectorFieldViewer
{
public:
    /**
     * @brief 显示二维三角形网格上的节点向量场
     *
     * @param mesh 三角形网格
     * @param vx 节点向量 x 分量，大小等于 mesh.num_nodes()
     * @param vy 节点向量 y 分量，大小等于 mesh.num_nodes()
     * @param title 窗口标题
     * @param scale_factor 箭头缩放系数
     */
    static void showVectorField2D(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& vx,
        const Eigen::VectorXd& vy,
        const char* title = "Vector Field",
        double scale_factor = 0.08
    );

    /**
     * @brief 显示二维节点向量场，同时以 |v| 作为颜色
     */
    static void showVectorField2DWithMagnitude(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& vx,
        const Eigen::VectorXd& vy,
        const char* title = "Vector Field Magnitude",
        double scale_factor = 0.08
    );
};

} // namespace OpenCAX
#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <Eigen/Dense>

#include <functional>

namespace OpenCAX
{

class OpenCAX_API ErrorNorms2D
{
public:
    using Function2D = std::function<double(double, double)>;

    using GradFunction2D =
        std::function<std::array<double, 2>(double, double)>;

public:
    /**
     * @brief 节点离散 l2 误差
     *
     * l2 = sqrt( sum_i (u_i - uh_i)^2 / N )
     */
    static double discreteL2(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& numerical,
        Function2D exact
    );

    /**
     * @brief 连续 L2 误差
     *
     * L2 = sqrt( ∫ (u - uh)^2 dx )
     *
     * 当前使用三角形重心一点评价。
     * 对 P1 初期收敛测试够用。
     */
    static double L2(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& numerical,
        Function2D exact
    );

    /**
     * @brief H1 半范数误差
     *
     * H1_semi = sqrt( ∫ |grad u - grad uh|^2 dx )
     */
    static double H1Semi(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& numerical,
        GradFunction2D exact_grad
    );

    /**
     * @brief H1 误差
     *
     * H1 = sqrt( L2^2 + H1Semi^2 )
     */
    static double H1(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& numerical,
        Function2D exact,
        GradFunction2D exact_grad
    );

    /**
     * @brief 计算网格尺寸 h
     *
     * h = max cell diameter
     */
    static double meshSize(
        const TriangleMesh& mesh
    );

private:
    static double triangleArea(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

    static double distance(
        const MeshNode& a,
        const MeshNode& b
    );
};

} // namespace OpenCAX
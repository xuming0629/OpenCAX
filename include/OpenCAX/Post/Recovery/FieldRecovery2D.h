#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <Eigen/Dense>

#include <array>
#include <functional>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 二维单元标量/向量后处理工具
 *
 * 当前主要用于 P1 三角形有限元：
 *
 * - 计算单元梯度 grad(u)
 * - 计算单元通量 q = -k grad(u)
 * - 计算 |grad(u)|
 * - 计算 |q|
 * - 将单元场平均恢复到节点场
 */
class OpenCAX_API FieldRecovery2D
{
public:
    using CoefficientFunction2D =
        std::function<double(double, double)>;

    struct CellGradient
    {
        int cell_id = -1;

        double cx = 0.0;
        double cy = 0.0;

        double dudx = 0.0;
        double dudy = 0.0;

        double magnitude = 0.0;
    };

    struct CellFlux
    {
        int cell_id = -1;

        double cx = 0.0;
        double cy = 0.0;

        double qx = 0.0;
        double qy = 0.0;

        double magnitude = 0.0;

        double coefficient = 1.0;
    };

public:
    /**
     * @brief 计算每个三角形单元上的 grad(u)
     *
     * P1 单元中 grad(u) 在每个单元内部是常数。
     */
    static std::vector<CellGradient> computeCellGradients(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& nodal_values
    );

    /**
     * @brief 计算每个三角形单元上的通量 q = -k grad(u)
     */
    static std::vector<CellFlux> computeCellFluxes(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& nodal_values,
        CoefficientFunction2D coefficient
    );

    /**
     * @brief 计算节点上的 |grad(u)|
     *
     * 方法：把相邻单元的 |grad(u)| 面积加权平均到节点。
     */
    static Eigen::VectorXd recoverNodalGradientMagnitude(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& nodal_values
    );

    /**
     * @brief 计算节点上的 |q|
     *
     * 方法：把相邻单元的 |q| 面积加权平均到节点。
     */
    static Eigen::VectorXd recoverNodalFluxMagnitude(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& nodal_values,
        CoefficientFunction2D coefficient
    );

    /**
     * @brief 恢复节点 qx
     */
    static Eigen::VectorXd recoverNodalFluxX(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& nodal_values,
        CoefficientFunction2D coefficient
    );

    /**
     * @brief 恢复节点 qy
     */
    static Eigen::VectorXd recoverNodalFluxY(
        const TriangleMesh& mesh,
        const Eigen::VectorXd& nodal_values,
        CoefficientFunction2D coefficient
    );

private:
    static double triangleArea(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

    static std::array<double, 2> triangleCentroid(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );
};

} // namespace OpenCAX
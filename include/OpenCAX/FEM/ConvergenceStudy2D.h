#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/ErrorNorms2D.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <Eigen/Dense>

#include <string>
#include <vector>

namespace OpenCAX
{

struct OpenCAX_API ConvergenceRecord2D
{
    int level = 0;

    std::size_t nodes = 0;
    std::size_t cells = 0;

    double h = 0.0;

    double l2 = 0.0;
    double L2 = 0.0;
    double H1Semi = 0.0;
    double H1 = 0.0;

    double order_l2 = 0.0;
    double order_L2 = 0.0;
    double order_H1Semi = 0.0;
    double order_H1 = 0.0;
};

class OpenCAX_API ConvergenceStudy2D
{
public:
    static ConvergenceRecord2D computeRecord(
        int level,
        const TriangleMesh& mesh,
        const Eigen::VectorXd& solution,
        ErrorNorms2D::Function2D exact,
        ErrorNorms2D::GradFunction2D exact_grad
    );

    static void computeOrders(
        std::vector<ConvergenceRecord2D>& records
    );

    static void print(
        const std::vector<ConvergenceRecord2D>& records
    );

    static bool writeCSV(
        const std::vector<ConvergenceRecord2D>& records,
        const std::string& filename
    );
};

} // namespace OpenCAX
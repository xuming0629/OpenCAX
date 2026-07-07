#include <OpenCAX/FEM/ConvergenceStudy2D.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace OpenCAX
{

ConvergenceRecord2D ConvergenceStudy2D::computeRecord(
    int level,
    const TriangleMesh& mesh,
    const Eigen::VectorXd& solution,
    ErrorNorms2D::Function2D exact,
    ErrorNorms2D::GradFunction2D exact_grad
)
{
    ConvergenceRecord2D r;

    r.level = level;
    r.nodes = mesh.num_nodes();
    r.cells = mesh.num_cells();

    r.h =
        ErrorNorms2D::meshSize(mesh);

    r.l2 =
        ErrorNorms2D::discreteL2(
            mesh,
            solution,
            exact
        );

    r.L2 =
        ErrorNorms2D::L2(
            mesh,
            solution,
            exact
        );

    r.H1Semi =
        ErrorNorms2D::H1Semi(
            mesh,
            solution,
            exact_grad
        );

    r.H1 =
        std::sqrt(
            r.L2 * r.L2 +
            r.H1Semi * r.H1Semi
        );

    return r;
}

void ConvergenceStudy2D::computeOrders(
    std::vector<ConvergenceRecord2D>& records
)
{
    if (records.size() < 2)
    {
        return;
    }

    records[0].order_l2 = 0.0;
    records[0].order_L2 = 0.0;
    records[0].order_H1Semi = 0.0;
    records[0].order_H1 = 0.0;

    for (std::size_t i = 1; i < records.size(); ++i)
    {
        const auto& prev = records[i - 1];
        auto& curr = records[i];

        const double denom =
            std::log(prev.h / curr.h);

        if (std::abs(denom) < 1.0e-14)
        {
            continue;
        }

        curr.order_l2 =
            std::log(prev.l2 / curr.l2) / denom;

        curr.order_L2 =
            std::log(prev.L2 / curr.L2) / denom;

        curr.order_H1Semi =
            std::log(prev.H1Semi / curr.H1Semi) / denom;

        curr.order_H1 =
            std::log(prev.H1 / curr.H1) / denom;
    }
}

void ConvergenceStudy2D::print(
    const std::vector<ConvergenceRecord2D>& records
)
{
    std::cout << "\n";
    std::cout << "==================== CONVERGENCE STUDY ====================\n";

    std::cout
        << std::setw(6)  << "level"
        << std::setw(10) << "nodes"
        << std::setw(10) << "cells"
        << std::setw(14) << "h"
        << std::setw(14) << "l2"
        << std::setw(10) << "ord"
        << std::setw(14) << "L2"
        << std::setw(10) << "ord"
        << std::setw(14) << "H1semi"
        << std::setw(10) << "ord"
        << std::setw(14) << "H1"
        << std::setw(10) << "ord"
        << "\n";

    for (const auto& r : records)
    {
        std::cout
            << std::setw(6)  << r.level
            << std::setw(10) << r.nodes
            << std::setw(10) << r.cells
            << std::setw(14) << std::scientific << r.h
            << std::setw(14) << std::scientific << r.l2
            << std::setw(10) << std::fixed << std::setprecision(3) << r.order_l2
            << std::setw(14) << std::scientific << r.L2
            << std::setw(10) << std::fixed << std::setprecision(3) << r.order_L2
            << std::setw(14) << std::scientific << r.H1Semi
            << std::setw(10) << std::fixed << std::setprecision(3) << r.order_H1Semi
            << std::setw(14) << std::scientific << r.H1
            << std::setw(10) << std::fixed << std::setprecision(3) << r.order_H1
            << "\n";
    }

    std::cout << "===========================================================\n\n";
}

bool ConvergenceStudy2D::writeCSV(
    const std::vector<ConvergenceRecord2D>& records,
    const std::string& filename
)
{
    std::ofstream fout(filename);

    if (!fout.is_open())
    {
        return false;
    }

    fout
        << "level,nodes,cells,h,"
        << "l2,order_l2,"
        << "L2,order_L2,"
        << "H1Semi,order_H1Semi,"
        << "H1,order_H1\n";

    for (const auto& r : records)
    {
        fout
            << r.level << ","
            << r.nodes << ","
            << r.cells << ","
            << r.h << ","
            << r.l2 << ","
            << r.order_l2 << ","
            << r.L2 << ","
            << r.order_L2 << ","
            << r.H1Semi << ","
            << r.order_H1Semi << ","
            << r.H1 << ","
            << r.order_H1
            << "\n";
    }

    return true;
}

} // namespace OpenCAX
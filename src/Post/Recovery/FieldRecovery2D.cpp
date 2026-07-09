#include <OpenCAX/Post/Recovery/FieldRecovery2D.h>

#include <OpenCAX/Mesh2/MeshTypes.h>

#include <cmath>
#include <iostream>

namespace OpenCAX
{

double FieldRecovery2D::triangleArea(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    return 0.5 * std::abs(
        (b.x - a.x) * (c.y - a.y) -
        (c.x - a.x) * (b.y - a.y)
    );
}

std::array<double, 2> FieldRecovery2D::triangleCentroid(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    return {
        (a.x + b.x + c.x) / 3.0,
        (a.y + b.y + c.y) / 3.0
    };
}

std::vector<FieldRecovery2D::CellGradient>
FieldRecovery2D::computeCellGradients(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& nodal_values
)
{
    std::vector<CellGradient> result;

    if (nodal_values.size() != static_cast<int>(mesh.num_nodes()))
    {
        std::cerr
            << "[OpenCAX::FieldRecovery2D] nodal value size mismatch."
            << std::endl;

        return result;
    }

    const auto& nodes =
        mesh.nodes();

    const auto& cells =
        mesh.cells();

    result.reserve(
        cells.size()
    );

    for (const auto& cell : cells)
    {
        if (cell.type != CellType::Triangle3)
        {
            continue;
        }

        if (cell.node_ids.size() != 3)
        {
            continue;
        }

        const int id0 =
            cell.node_ids[0];

        const int id1 =
            cell.node_ids[1];

        const int id2 =
            cell.node_ids[2];

        const MeshNode& p0 =
            nodes[static_cast<std::size_t>(id0)];

        const MeshNode& p1 =
            nodes[static_cast<std::size_t>(id1)];

        const MeshNode& p2 =
            nodes[static_cast<std::size_t>(id2)];

        const double area =
            triangleArea(
                p0,
                p1,
                p2
            );

        if (area < 1.0e-14)
        {
            continue;
        }

        const double twoA =
            2.0 * area;

        double b[3];
        double c[3];

        b[0] = (p1.y - p2.y) / twoA;
        b[1] = (p2.y - p0.y) / twoA;
        b[2] = (p0.y - p1.y) / twoA;

        c[0] = (p2.x - p1.x) / twoA;
        c[1] = (p0.x - p2.x) / twoA;
        c[2] = (p1.x - p0.x) / twoA;

        const double u0 =
            nodal_values(id0);

        const double u1 =
            nodal_values(id1);

        const double u2 =
            nodal_values(id2);

        const double dudx =
            u0 * b[0] +
            u1 * b[1] +
            u2 * b[2];

        const double dudy =
            u0 * c[0] +
            u1 * c[1] +
            u2 * c[2];

        const auto center =
            triangleCentroid(
                p0,
                p1,
                p2
            );

        CellGradient grad;

        grad.cell_id =
            cell.id;

        grad.cx =
            center[0];

        grad.cy =
            center[1];

        grad.dudx =
            dudx;

        grad.dudy =
            dudy;

        grad.magnitude =
            std::sqrt(
                dudx * dudx +
                dudy * dudy
            );

        result.push_back(
            grad
        );
    }

    return result;
}

std::vector<FieldRecovery2D::CellFlux>
FieldRecovery2D::computeCellFluxes(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& nodal_values,
    CoefficientFunction2D coefficient
)
{
    std::vector<CellFlux> result;

    const auto gradients =
        computeCellGradients(
            mesh,
            nodal_values
        );

    result.reserve(
        gradients.size()
    );

    for (const auto& grad : gradients)
    {
        const double k =
            coefficient(
                grad.cx,
                grad.cy
            );

        CellFlux flux;

        flux.cell_id =
            grad.cell_id;

        flux.cx =
            grad.cx;

        flux.cy =
            grad.cy;

        flux.coefficient =
            k;

        flux.qx =
            -k * grad.dudx;

        flux.qy =
            -k * grad.dudy;

        flux.magnitude =
            std::sqrt(
                flux.qx * flux.qx +
                flux.qy * flux.qy
            );

        result.push_back(
            flux
        );
    }

    return result;
}

Eigen::VectorXd FieldRecovery2D::recoverNodalGradientMagnitude(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& nodal_values
)
{
    Eigen::VectorXd nodal =
        Eigen::VectorXd::Zero(
            static_cast<int>(mesh.num_nodes())
        );

    Eigen::VectorXd weight =
        Eigen::VectorXd::Zero(
            static_cast<int>(mesh.num_nodes())
        );

    const auto& nodes =
        mesh.nodes();

    const auto& cells =
        mesh.cells();

    const auto gradients =
        computeCellGradients(
            mesh,
            nodal_values
        );

    std::size_t gi = 0;

    for (const auto& cell : cells)
    {
        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3)
        {
            continue;
        }

        if (gi >= gradients.size())
        {
            break;
        }

        const int id0 =
            cell.node_ids[0];

        const int id1 =
            cell.node_ids[1];

        const int id2 =
            cell.node_ids[2];

        const MeshNode& p0 =
            nodes[static_cast<std::size_t>(id0)];

        const MeshNode& p1 =
            nodes[static_cast<std::size_t>(id1)];

        const MeshNode& p2 =
            nodes[static_cast<std::size_t>(id2)];

        const double area =
            triangleArea(
                p0,
                p1,
                p2
            );

        const double value =
            gradients[gi].magnitude;

        nodal(id0) +=
            area * value;

        nodal(id1) +=
            area * value;

        nodal(id2) +=
            area * value;

        weight(id0) +=
            area;

        weight(id1) +=
            area;

        weight(id2) +=
            area;

        ++gi;
    }

    for (int i = 0; i < nodal.size(); ++i)
    {
        if (weight(i) > 0.0)
        {
            nodal(i) /=
                weight(i);
        }
    }

    return nodal;
}

Eigen::VectorXd FieldRecovery2D::recoverNodalFluxMagnitude(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& nodal_values,
    CoefficientFunction2D coefficient
)
{
    Eigen::VectorXd nodal =
        Eigen::VectorXd::Zero(
            static_cast<int>(mesh.num_nodes())
        );

    Eigen::VectorXd weight =
        Eigen::VectorXd::Zero(
            static_cast<int>(mesh.num_nodes())
        );

    const auto& nodes =
        mesh.nodes();

    const auto& cells =
        mesh.cells();

    const auto fluxes =
        computeCellFluxes(
            mesh,
            nodal_values,
            coefficient
        );

    std::size_t fi = 0;

    for (const auto& cell : cells)
    {
        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3)
        {
            continue;
        }

        if (fi >= fluxes.size())
        {
            break;
        }

        const int id0 =
            cell.node_ids[0];

        const int id1 =
            cell.node_ids[1];

        const int id2 =
            cell.node_ids[2];

        const MeshNode& p0 =
            nodes[static_cast<std::size_t>(id0)];

        const MeshNode& p1 =
            nodes[static_cast<std::size_t>(id1)];

        const MeshNode& p2 =
            nodes[static_cast<std::size_t>(id2)];

        const double area =
            triangleArea(
                p0,
                p1,
                p2
            );

        const double value =
            fluxes[fi].magnitude;

        nodal(id0) +=
            area * value;

        nodal(id1) +=
            area * value;

        nodal(id2) +=
            area * value;

        weight(id0) +=
            area;

        weight(id1) +=
            area;

        weight(id2) +=
            area;

        ++fi;
    }

    for (int i = 0; i < nodal.size(); ++i)
    {
        if (weight(i) > 0.0)
        {
            nodal(i) /=
                weight(i);
        }
    }

    return nodal;
}

Eigen::VectorXd FieldRecovery2D::recoverNodalFluxX(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& nodal_values,
    CoefficientFunction2D coefficient
)
{
    Eigen::VectorXd nodal =
        Eigen::VectorXd::Zero(
            static_cast<int>(mesh.num_nodes())
        );

    Eigen::VectorXd weight =
        Eigen::VectorXd::Zero(
            static_cast<int>(mesh.num_nodes())
        );

    const auto& nodes =
        mesh.nodes();

    const auto& cells =
        mesh.cells();

    const auto fluxes =
        computeCellFluxes(
            mesh,
            nodal_values,
            coefficient
        );

    std::size_t fi = 0;

    for (const auto& cell : cells)
    {
        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3)
        {
            continue;
        }

        if (fi >= fluxes.size())
        {
            break;
        }

        const int id0 =
            cell.node_ids[0];

        const int id1 =
            cell.node_ids[1];

        const int id2 =
            cell.node_ids[2];

        const MeshNode& p0 =
            nodes[static_cast<std::size_t>(id0)];

        const MeshNode& p1 =
            nodes[static_cast<std::size_t>(id1)];

        const MeshNode& p2 =
            nodes[static_cast<std::size_t>(id2)];

        const double area =
            triangleArea(
                p0,
                p1,
                p2
            );

        const double value =
            fluxes[fi].qx;

        nodal(id0) +=
            area * value;

        nodal(id1) +=
            area * value;

        nodal(id2) +=
            area * value;

        weight(id0) +=
            area;

        weight(id1) +=
            area;

        weight(id2) +=
            area;

        ++fi;
    }

    for (int i = 0; i < nodal.size(); ++i)
    {
        if (weight(i) > 0.0)
        {
            nodal(i) /=
                weight(i);
        }
    }

    return nodal;
}

Eigen::VectorXd FieldRecovery2D::recoverNodalFluxY(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& nodal_values,
    CoefficientFunction2D coefficient
)
{
    Eigen::VectorXd nodal =
        Eigen::VectorXd::Zero(
            static_cast<int>(mesh.num_nodes())
        );

    Eigen::VectorXd weight =
        Eigen::VectorXd::Zero(
            static_cast<int>(mesh.num_nodes())
        );

    const auto& nodes =
        mesh.nodes();

    const auto& cells =
        mesh.cells();

    const auto fluxes =
        computeCellFluxes(
            mesh,
            nodal_values,
            coefficient
        );

    std::size_t fi = 0;

    for (const auto& cell : cells)
    {
        if (cell.type != CellType::Triangle3 ||
            cell.node_ids.size() != 3)
        {
            continue;
        }

        if (fi >= fluxes.size())
        {
            break;
        }

        const int id0 =
            cell.node_ids[0];

        const int id1 =
            cell.node_ids[1];

        const int id2 =
            cell.node_ids[2];

        const MeshNode& p0 =
            nodes[static_cast<std::size_t>(id0)];

        const MeshNode& p1 =
            nodes[static_cast<std::size_t>(id1)];

        const MeshNode& p2 =
            nodes[static_cast<std::size_t>(id2)];

        const double area =
            triangleArea(
                p0,
                p1,
                p2
            );

        const double value =
            fluxes[fi].qy;

        nodal(id0) +=
            area * value;

        nodal(id1) +=
            area * value;

        nodal(id2) +=
            area * value;

        weight(id0) +=
            area;

        weight(id1) +=
            area;

        weight(id2) +=
            area;

        ++fi;
    }

    for (int i = 0; i < nodal.size(); ++i)
    {
        if (weight(i) > 0.0)
        {
            nodal(i) /=
                weight(i);
        }
    }

    return nodal;
}

} // namespace OpenCAX
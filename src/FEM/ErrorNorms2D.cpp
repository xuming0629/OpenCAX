#include <OpenCAX/FEM/ErrorNorms2D.h>

#include <OpenCAX/Mesh/MeshTypes.h>

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace OpenCAX
{

double ErrorNorms2D::distance(
    const MeshNode& a,
    const MeshNode& b
)
{
    const double dx = a.x - b.x;
    const double dy = a.y - b.y;
    const double dz = a.z - b.z;

    return std::sqrt(
        dx * dx +
        dy * dy +
        dz * dz
    );
}

double ErrorNorms2D::triangleArea(
    const MeshNode& a,
    const MeshNode& b,
    const MeshNode& c
)
{
    const double x0 = a.x;
    const double y0 = a.y;

    const double x1 = b.x;
    const double y1 = b.y;

    const double x2 = c.x;
    const double y2 = c.y;

    const double detJ =
        (x1 - x0) * (y2 - y0) -
        (x2 - x0) * (y1 - y0);

    return 0.5 * std::abs(detJ);
}

double ErrorNorms2D::meshSize(
    const TriangleMesh& mesh
)
{
    const auto& nodes = mesh.nodes();
    const auto& cells = mesh.cells();

    double h = 0.0;

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

        const MeshNode& p0 = nodes[cell.node_ids[0]];
        const MeshNode& p1 = nodes[cell.node_ids[1]];
        const MeshNode& p2 = nodes[cell.node_ids[2]];

        h = std::max(h, distance(p0, p1));
        h = std::max(h, distance(p1, p2));
        h = std::max(h, distance(p2, p0));
    }

    return h;
}

double ErrorNorms2D::discreteL2(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& numerical,
    Function2D exact
)
{
    const auto& nodes = mesh.nodes();

    if (numerical.size() != static_cast<int>(nodes.size()))
    {
        throw std::runtime_error(
            "[OpenCAX::ErrorNorms2D] solution size mismatch in discreteL2."
        );
    }

    double sum = 0.0;

    for (std::size_t i = 0; i < nodes.size(); ++i)
    {
        const MeshNode& node = nodes[i];

        const double ue =
            exact(node.x, node.y);

        const double uh =
            numerical(static_cast<int>(i));

        const double e = ue - uh;

        sum += e * e;
    }

    if (nodes.empty())
    {
        return 0.0;
    }

    return std::sqrt(
        sum / static_cast<double>(nodes.size())
    );
}

double ErrorNorms2D::L2(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& numerical,
    Function2D exact
)
{
    const auto& nodes = mesh.nodes();
    const auto& cells = mesh.cells();

    if (numerical.size() != static_cast<int>(nodes.size()))
    {
        throw std::runtime_error(
            "[OpenCAX::ErrorNorms2D] solution size mismatch in L2."
        );
    }

    double error2 = 0.0;

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

        const int id0 = cell.node_ids[0];
        const int id1 = cell.node_ids[1];
        const int id2 = cell.node_ids[2];

        const MeshNode& p0 = nodes[id0];
        const MeshNode& p1 = nodes[id1];
        const MeshNode& p2 = nodes[id2];

        const double area =
            triangleArea(p0, p1, p2);

        if (area < 1.0e-14)
        {
            continue;
        }

        const double xc =
            (p0.x + p1.x + p2.x) / 3.0;

        const double yc =
            (p0.y + p1.y + p2.y) / 3.0;

        const double ue =
            exact(xc, yc);

        const double uh =
            (
                numerical(id0) +
                numerical(id1) +
                numerical(id2)
            ) / 3.0;

        const double e = ue - uh;

        error2 += area * e * e;
    }

    return std::sqrt(error2);
}

double ErrorNorms2D::H1Semi(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& numerical,
    GradFunction2D exact_grad
)
{
    const auto& nodes = mesh.nodes();
    const auto& cells = mesh.cells();

    if (numerical.size() != static_cast<int>(nodes.size()))
    {
        throw std::runtime_error(
            "[OpenCAX::ErrorNorms2D] solution size mismatch in H1Semi."
        );
    }

    double error2 = 0.0;

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

        const int id0 = cell.node_ids[0];
        const int id1 = cell.node_ids[1];
        const int id2 = cell.node_ids[2];

        const MeshNode& p0 = nodes[id0];
        const MeshNode& p1 = nodes[id1];
        const MeshNode& p2 = nodes[id2];

        const double x0 = p0.x;
        const double y0 = p0.y;

        const double x1 = p1.x;
        const double y1 = p1.y;

        const double x2 = p2.x;
        const double y2 = p2.y;

        const double detJ =
            (x1 - x0) * (y2 - y0) -
            (x2 - x0) * (y1 - y0);

        const double area =
            0.5 * std::abs(detJ);

        if (area < 1.0e-14)
        {
            continue;
        }

        const double twoA =
            2.0 * area;

        double b[3];
        double c[3];

        b[0] = (y1 - y2) / twoA;
        b[1] = (y2 - y0) / twoA;
        b[2] = (y0 - y1) / twoA;

        c[0] = (x2 - x1) / twoA;
        c[1] = (x0 - x2) / twoA;
        c[2] = (x1 - x0) / twoA;

        const double u0 = numerical(id0);
        const double u1 = numerical(id1);
        const double u2 = numerical(id2);

        const double uh_x =
            u0 * b[0] +
            u1 * b[1] +
            u2 * b[2];

        const double uh_y =
            u0 * c[0] +
            u1 * c[1] +
            u2 * c[2];

        const double xc =
            (x0 + x1 + x2) / 3.0;

        const double yc =
            (y0 + y1 + y2) / 3.0;

        const auto gu =
            exact_grad(xc, yc);

        const double ex =
            gu[0] - uh_x;

        const double ey =
            gu[1] - uh_y;

        error2 += area * (ex * ex + ey * ey);
    }

    return std::sqrt(error2);
}

double ErrorNorms2D::H1(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& numerical,
    Function2D exact,
    GradFunction2D exact_grad
)
{
    const double eL2 =
        L2(mesh, numerical, exact);

    const double eH1Semi =
        H1Semi(mesh, numerical, exact_grad);

    return std::sqrt(
        eL2 * eL2 +
        eH1Semi * eH1Semi
    );
}

} // namespace OpenCAX
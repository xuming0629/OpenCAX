#pragma once

#include <OpenCAX/Core/Config.h>

#include <string>

namespace OpenCAX
{

enum class FEFamily
{
    Unknown,
    Lagrange
};

enum class FEOrder
{
    P0 = 0,
    P1 = 1,
    P2 = 2
};

enum class PDEType
{
    Unknown,
    Poisson,
    Heat,
    Elasticity
};

enum class BoundaryType
{
    Dirichlet,
    Neumann,
    Robin
};

enum class SolverType
{
    SparseLU,
    CG,
    BiCGSTAB
};

inline std::string to_string(FEOrder order)
{
    switch (order)
    {
    case FEOrder::P0:
        return "P0";
    case FEOrder::P1:
        return "P1";
    case FEOrder::P2:
        return "P2";
    default:
        return "Unknown";
    }
}

inline std::string to_string(SolverType type)
{
    switch (type)
    {
    case SolverType::SparseLU:
        return "SparseLU";
    case SolverType::CG:
        return "CG";
    case SolverType::BiCGSTAB:
        return "BiCGSTAB";
    default:
        return "Unknown";
    }
}

} // namespace OpenCAX

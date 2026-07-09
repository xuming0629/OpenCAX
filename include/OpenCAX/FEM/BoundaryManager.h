#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/FETypes.h>
#include <OpenCAX/Mesh/MeshTypes.h>

#include <functional>
#include <vector>

namespace OpenCAX
{

using Function2D = std::function<double(double, double)>;
using BoundaryPredicate2D = std::function<bool(double, double)>;

enum class BoundarySelectorType
{
    AllBoundary,
    Predicate,
    BoundaryId,
    PhysicalId
};

struct OpenCAX_API BoundarySelector2D
{
    BoundarySelectorType selector_type = BoundarySelectorType::AllBoundary;
    int id = -1;
    BoundaryPredicate2D predicate;

    static BoundarySelector2D all();
    static BoundarySelector2D byPredicate(BoundaryPredicate2D pred);
    static BoundarySelector2D byBoundaryId(int boundary_id);
    static BoundarySelector2D byPhysicalId(int physical_id);

    bool matchPoint(const MeshNode& node) const;
    bool matchEdge(const MeshNode& a, const MeshNode& b) const;
};

class OpenCAX_API BoundaryCondition2D
{
public:
    BoundaryCondition2D();
    BoundaryCondition2D(BoundaryType type, BoundarySelector2D selector, Function2D value);

    BoundaryType type() const;
    const BoundarySelector2D& selector() const;

    bool matchNode(const MeshNode& node) const;
    bool matchEdge(const MeshNode& a, const MeshNode& b) const;
    double value(double x, double y) const;

private:
    BoundaryType type_ = BoundaryType::Dirichlet;
    BoundarySelector2D selector_;
    Function2D value_;
};

class OpenCAX_API BoundaryManager2D
{
public:
    void clear();

    void addDirichlet(BoundarySelector2D selector, Function2D value);
    void addNeumann(BoundarySelector2D selector, Function2D value);
    void addRobin(BoundarySelector2D selector, double alpha, double beta, Function2D value);

    const std::vector<BoundaryCondition2D>& dirichlet() const;
    const std::vector<BoundaryCondition2D>& neumann() const;
    const std::vector<BoundaryCondition2D>& robin() const;

    bool dirichletValue(const MeshNode& node, double& value) const;

private:
    std::vector<BoundaryCondition2D> dirichlet_bcs_;
    std::vector<BoundaryCondition2D> neumann_bcs_;
    std::vector<BoundaryCondition2D> robin_bcs_;
};

} // namespace OpenCAX

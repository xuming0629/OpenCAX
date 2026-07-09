#include <OpenCAX/FEM/BoundaryManager.h>

#include <cmath>
#include <utility>

namespace OpenCAX
{

BoundarySelector2D BoundarySelector2D::all()
{
    BoundarySelector2D s;
    s.selector_type = BoundarySelectorType::AllBoundary;
    return s;
}

BoundarySelector2D BoundarySelector2D::byPredicate(BoundaryPredicate2D pred)
{
    BoundarySelector2D s;
    s.selector_type = BoundarySelectorType::Predicate;
    s.predicate = std::move(pred);
    return s;
}

BoundarySelector2D BoundarySelector2D::byBoundaryId(int boundary_id)
{
    BoundarySelector2D s;
    s.selector_type = BoundarySelectorType::BoundaryId;
    s.id = boundary_id;
    return s;
}

BoundarySelector2D BoundarySelector2D::byPhysicalId(int physical_id)
{
    BoundarySelector2D s;
    s.selector_type = BoundarySelectorType::PhysicalId;
    s.id = physical_id;
    return s;
}

bool BoundarySelector2D::matchPoint(const MeshNode& node) const
{
    switch (selector_type)
    {
    case BoundarySelectorType::AllBoundary:
        return true;
    case BoundarySelectorType::Predicate:
        return predicate ? predicate(node.x, node.y) : false;
    case BoundarySelectorType::BoundaryId:
        return node.boundary_id == id;
    case BoundarySelectorType::PhysicalId:
        return node.physical_id == id;
    default:
        return false;
    }
}

bool BoundarySelector2D::matchEdge(const MeshNode& a, const MeshNode& b) const
{
    const double mx = 0.5 * (a.x + b.x);
    const double my = 0.5 * (a.y + b.y);

    switch (selector_type)
    {
    case BoundarySelectorType::AllBoundary:
        return true;
    case BoundarySelectorType::Predicate:
        return predicate ? predicate(mx, my) : false;
    case BoundarySelectorType::BoundaryId:
        return a.boundary_id == id && b.boundary_id == id;
    case BoundarySelectorType::PhysicalId:
        return a.physical_id == id && b.physical_id == id;
    default:
        return false;
    }
}

BoundaryCondition2D::BoundaryCondition2D()
{
    value_ = [](double, double) { return 0.0; };
}

BoundaryCondition2D::BoundaryCondition2D(BoundaryType type, BoundarySelector2D selector, Function2D value)
    : type_(type), selector_(std::move(selector)), value_(std::move(value))
{
}

BoundaryType BoundaryCondition2D::type() const { return type_; }
const BoundarySelector2D& BoundaryCondition2D::selector() const { return selector_; }
bool BoundaryCondition2D::matchNode(const MeshNode& node) const { return selector_.matchPoint(node); }
bool BoundaryCondition2D::matchEdge(const MeshNode& a, const MeshNode& b) const { return selector_.matchEdge(a, b); }
double BoundaryCondition2D::value(double x, double y) const { return value_(x, y); }

void BoundaryManager2D::clear()
{
    dirichlet_bcs_.clear();
    neumann_bcs_.clear();
    robin_bcs_.clear();
}

void BoundaryManager2D::addDirichlet(BoundarySelector2D selector, Function2D value)
{
    dirichlet_bcs_.emplace_back(BoundaryType::Dirichlet, std::move(selector), std::move(value));
}

void BoundaryManager2D::addNeumann(BoundarySelector2D selector, Function2D value)
{
    neumann_bcs_.emplace_back(BoundaryType::Neumann, std::move(selector), std::move(value));
}

void BoundaryManager2D::addRobin(BoundarySelector2D selector, double, double, Function2D value)
{
    robin_bcs_.emplace_back(BoundaryType::Robin, std::move(selector), std::move(value));
}

const std::vector<BoundaryCondition2D>& BoundaryManager2D::dirichlet() const { return dirichlet_bcs_; }
const std::vector<BoundaryCondition2D>& BoundaryManager2D::neumann() const { return neumann_bcs_; }
const std::vector<BoundaryCondition2D>& BoundaryManager2D::robin() const { return robin_bcs_; }

bool BoundaryManager2D::dirichletValue(const MeshNode& node, double& value) const
{
    for (const auto& bc : dirichlet_bcs_)
    {
        if (bc.matchNode(node))
        {
            value = bc.value(node.x, node.y);
            return true;
        }
    }
    return false;
}

} // namespace OpenCAX

#include <OpenCAX/FEM/PhysicsProblem2D.h>
#include <OpenCAX/FEM/SparseLUSolver.h>

#include <iostream>

namespace OpenCAX
{

PhysicsProblem2D::PhysicsProblem2D(const P1TriangleSpace& space)
    : space_(space)
{
    solver_ = std::make_shared<SparseLUSolver>();
}

const P1TriangleSpace& PhysicsProblem2D::space() const { return space_; }
LinearSystem& PhysicsProblem2D::linearSystem() { return system_; }
const LinearSystem& PhysicsProblem2D::linearSystem() const { return system_; }
const Eigen::VectorXd& PhysicsProblem2D::solution() const { return system_.solution(); }
MaterialManager2D& PhysicsProblem2D::materials() { return materials_; }
const MaterialManager2D& PhysicsProblem2D::materials() const { return materials_; }
BoundaryManager2D& PhysicsProblem2D::boundaries() { return boundaries_; }
const BoundaryManager2D& PhysicsProblem2D::boundaries() const { return boundaries_; }

void PhysicsProblem2D::setSolver(std::shared_ptr<LinearSolver> solver)
{
    solver_ = std::move(solver);
}

std::shared_ptr<LinearSolver> PhysicsProblem2D::solver() const
{
    return solver_;
}

bool PhysicsProblem2D::solve()
{
    assemble();
    applyBoundaryConditions();

    if (!solver_)
    {
        std::cerr << "[OpenCAX::PhysicsProblem2D] solver is null.\n";
        return false;
    }

    return solver_->solve(system_);
}

} // namespace OpenCAX

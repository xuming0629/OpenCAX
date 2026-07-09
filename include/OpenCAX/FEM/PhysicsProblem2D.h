#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/BoundaryManager.h>
#include <OpenCAX/FEM/LinearSolver.h>
#include <OpenCAX/FEM/LinearSystem.h>
#include <OpenCAX/FEM/Material.h>
#include <OpenCAX/FEM/P1TriangleSpace.h>

#include <Eigen/Dense>

#include <memory>

namespace OpenCAX
{

class OpenCAX_API PhysicsProblem2D
{
public:
    explicit PhysicsProblem2D(const P1TriangleSpace& space);
    virtual ~PhysicsProblem2D() = default;

    const P1TriangleSpace& space() const;

    LinearSystem& linearSystem();
    const LinearSystem& linearSystem() const;
    const Eigen::VectorXd& solution() const;

    MaterialManager2D& materials();
    const MaterialManager2D& materials() const;

    BoundaryManager2D& boundaries();
    const BoundaryManager2D& boundaries() const;

    void setSolver(std::shared_ptr<LinearSolver> solver);
    std::shared_ptr<LinearSolver> solver() const;

    virtual void assemble() = 0;
    virtual void applyBoundaryConditions() = 0;
    virtual bool solve();

protected:
    const P1TriangleSpace& space_;
    LinearSystem system_;
    MaterialManager2D materials_;
    BoundaryManager2D boundaries_;
    std::shared_ptr<LinearSolver> solver_;
};

} // namespace OpenCAX

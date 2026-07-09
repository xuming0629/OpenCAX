#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/BoundaryManager.h>
#include <OpenCAX/FEM/ElasticMaterial2D.h>
#include <OpenCAX/FEM/LinearSolver.h>
#include <OpenCAX/FEM/LinearSystem.h>
#include <OpenCAX/FEM/P1TriangleSpace.h>

#include <Eigen/Dense>

#include <functional>
#include <memory>
#include <vector>

namespace OpenCAX
{

using VectorFunction2D = std::function<Eigen::Vector2d(double, double)>;

class OpenCAX_API Elasticity2D
{
public:
    explicit Elasticity2D(const P1TriangleSpace& space);

    void setMaterial(const LinearElasticMaterial2D& material);
    void setBodyForce(VectorFunction2D body_force);

    void addDirichletBC(BoundarySelector2D selector, VectorFunction2D displacement);
    void addTractionBC(BoundarySelector2D selector, VectorFunction2D traction);

    void setSolver(std::shared_ptr<LinearSolver> solver);

    void assemble();
    void applyBoundaryConditions();
    bool solve();

    const Eigen::VectorXd& displacement() const;
    Eigen::Vector2d nodeDisplacement(int node_id) const;

    LinearSystem& linearSystem();
    const LinearSystem& linearSystem() const;

private:
    struct VectorBC
    {
        BoundarySelector2D selector;
        VectorFunction2D value;
    };

    int uxDof(int node_id) const;
    int uyDof(int node_id) const;

    void assembleCell(std::size_t cell_id, std::vector<Eigen::Triplet<double>>& triplets);
    void applyTractionBC();
    void applyDirichletSparse();

private:
    const P1TriangleSpace& space_;
    LinearElasticMaterial2D material_;
    VectorFunction2D body_force_;
    std::vector<VectorBC> dirichlet_bcs_;
    std::vector<VectorBC> traction_bcs_;
    LinearSystem system_;
    std::shared_ptr<LinearSolver> solver_;
};

} // namespace OpenCAX

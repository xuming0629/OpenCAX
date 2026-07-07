#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/BoundaryCondition.h>
#include <OpenCAX/FEM/LinearSystem.h>
#include <OpenCAX/FEM/LinearSolver.h>
#include <OpenCAX/FEM/P1TriangleSpace.h>
#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <memory>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API Poisson2D
{
public:
    explicit Poisson2D(
        const TriangleMesh& mesh
    );

    Poisson2D(
        const TriangleMesh& mesh,
        const MeshTopology& topology
    );

    explicit Poisson2D(
        const P1TriangleSpace& space
    );

    void setSource(
        Function2D f
    );

    void setDirichlet(
        Function2D g
    );

    void setSolver(
        std::shared_ptr<LinearSolver> solver
    );

    const P1TriangleSpace& space() const;

    LinearSystem& linearSystem();

    const LinearSystem& linearSystem() const;

    const Eigen::VectorXd& solution() const;

    void assemble();

    void applyDirichlet();

    bool solve();

private:
    void assembleP1();

    void assembleP1Cell(
        std::size_t cell_id,
        std::vector<Eigen::Triplet<double>>& triplets
    );

    void applyDirichletSparse();

private:
    std::unique_ptr<P1TriangleSpace> owned_space_;
    const P1TriangleSpace* space_ = nullptr;

    LinearSystem system_;

    Function2D source_;
    BoundaryCondition2D dirichlet_;

    std::shared_ptr<LinearSolver> solver_;
};

} // namespace OpenCAX
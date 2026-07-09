#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/FEM/PhysicsProblem2D.h>

#include <Eigen/Sparse>

namespace OpenCAX
{

enum class TimeScheme
{
    BackwardEuler
};

class OpenCAX_API HeatEquation2D : public PhysicsProblem2D
{
public:
    explicit HeatEquation2D(const P1TriangleSpace& space);

    void setCapacity(double rho_cp);
    void setConductivity(double k);
    void setConductivity(ScalarFunction2D k);
    void setSource(Function2D source);
    void setInitialCondition(Function2D initial);

    void addDirichletBC(BoundarySelector2D selector, Function2D value);
    void addNeumannBC(BoundarySelector2D selector, Function2D value);

    void initialize();
    bool step(double dt);
    bool run(double dt, int steps);

    double time() const;

    void assemble() override;
    void applyBoundaryConditions() override;

private:
    void assembleMassAndStiffness();
    void assembleNeumann(Eigen::VectorXd& rhs) const;
    void applyDirichletToSystem(Eigen::SparseMatrix<double>& A, Eigen::VectorXd& rhs);

private:
    double time_ = 0.0;
    double capacity_ = 1.0;
    Function2D source_;
    Function2D initial_;

    Eigen::SparseMatrix<double> M_;
    Eigen::SparseMatrix<double> K_;
    Eigen::VectorXd F_;
    Eigen::VectorXd U_old_;
};

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <Eigen/Sparse>
#include <Eigen/Dense>

#include <functional>

namespace OpenCAX
{

class OpenCAX_API Poisson2D
{
public:
    using Function2D = std::function<double(double, double)>;

public:
    explicit Poisson2D(const TriangleMesh& mesh);

    void setSource(Function2D f);
    void setDirichlet(Function2D g);

    bool solve();

    const Eigen::VectorXd& solution() const;

private:
    void assemble();
    void applyDirichlet();

private:
    const TriangleMesh& mesh_;

    Function2D source_;
    Function2D dirichlet_;

    Eigen::SparseMatrix<double> K_;
    Eigen::VectorXd F_;
    Eigen::VectorXd U_;
};

} // namespace OpenCAX
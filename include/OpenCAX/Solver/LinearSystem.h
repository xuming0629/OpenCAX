#pragma once
#include <Eigen/Sparse>
#include <Eigen/Core>
#include <stdexcept>
namespace OpenCAX {
class LinearSystem {
public:
    using Matrix = Eigen::SparseMatrix<double>;
    using Vector = Eigen::VectorXd;
    void resize(int n) {
        if (n < 0) throw std::invalid_argument("LinearSystem::resize: size must be non-negative");
        A.resize(n,n); b.resize(n); x.resize(n); b.setZero(); x.setZero();
    }
    int size() const { return static_cast<int>(b.size()); }
    bool valid() const { return A.rows()==A.cols() && A.rows()==b.size() && x.size()==b.size(); }
    Matrix A; Vector b; Vector x;
};
} // namespace OpenCAX

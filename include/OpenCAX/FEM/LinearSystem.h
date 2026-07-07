#pragma once

#include <OpenCAX/Core/Config.h>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <cstddef>

namespace OpenCAX
{

class OpenCAX_API LinearSystem
{
public:
    using SparseMatrix = Eigen::SparseMatrix<double>;
    using Vector = Eigen::VectorXd;

public:
    LinearSystem() = default;

    explicit LinearSystem(std::size_t n)
    {
        resize(n);
    }

    void resize(std::size_t n)
    {
        K_.resize(static_cast<int>(n), static_cast<int>(n));
        F_ = Vector::Zero(static_cast<int>(n));
        U_ = Vector::Zero(static_cast<int>(n));
    }

    void clear()
    {
        K_.resize(0, 0);
        F_.resize(0);
        U_.resize(0);
    }

    std::size_t size() const
    {
        return static_cast<std::size_t>(F_.size());
    }

    SparseMatrix& matrix()
    {
        return K_;
    }

    const SparseMatrix& matrix() const
    {
        return K_;
    }

    Vector& rhs()
    {
        return F_;
    }

    const Vector& rhs() const
    {
        return F_;
    }

    Vector& solution()
    {
        return U_;
    }

    const Vector& solution() const
    {
        return U_;
    }

private:
    SparseMatrix K_;
    Vector F_;
    Vector U_;
};

} // namespace OpenCAX

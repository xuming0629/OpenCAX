#include <OpenCAX/FEM/ElasticMaterial2D.h>

namespace OpenCAX
{

LinearElasticMaterial2D::LinearElasticMaterial2D() = default;

LinearElasticMaterial2D::LinearElasticMaterial2D(double young, double poisson, ElasticModel2D model)
    : E_(young), nu_(poisson), model_(model)
{
}

void LinearElasticMaterial2D::setYoungModulus(double E) { E_ = E; }
void LinearElasticMaterial2D::setPoissonRatio(double nu) { nu_ = nu; }
void LinearElasticMaterial2D::setModel(ElasticModel2D model) { model_ = model; }
double LinearElasticMaterial2D::youngModulus() const { return E_; }
double LinearElasticMaterial2D::poissonRatio() const { return nu_; }
ElasticModel2D LinearElasticMaterial2D::model() const { return model_; }

Eigen::Matrix3d LinearElasticMaterial2D::constitutiveMatrix() const
{
    Eigen::Matrix3d D = Eigen::Matrix3d::Zero();

    if (model_ == ElasticModel2D::PlaneStress)
    {
        const double c = E_ / (1.0 - nu_ * nu_);
        D(0, 0) = c;
        D(0, 1) = c * nu_;
        D(1, 0) = c * nu_;
        D(1, 1) = c;
        D(2, 2) = c * (1.0 - nu_) / 2.0;
    }
    else
    {
        const double c = E_ / ((1.0 + nu_) * (1.0 - 2.0 * nu_));
        D(0, 0) = c * (1.0 - nu_);
        D(0, 1) = c * nu_;
        D(1, 0) = c * nu_;
        D(1, 1) = c * (1.0 - nu_);
        D(2, 2) = c * (1.0 - 2.0 * nu_) / 2.0;
    }

    return D;
}

} // namespace OpenCAX

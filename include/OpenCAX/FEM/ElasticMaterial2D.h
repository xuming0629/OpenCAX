#pragma once

#include <OpenCAX/Core/Config.h>
#include <Eigen/Dense>

namespace OpenCAX
{

enum class ElasticModel2D
{
    PlaneStress,
    PlaneStrain
};

class OpenCAX_API LinearElasticMaterial2D
{
public:
    LinearElasticMaterial2D();
    LinearElasticMaterial2D(double young, double poisson, ElasticModel2D model = ElasticModel2D::PlaneStress);

    void setYoungModulus(double E);
    void setPoissonRatio(double nu);
    void setModel(ElasticModel2D model);

    double youngModulus() const;
    double poissonRatio() const;
    ElasticModel2D model() const;

    Eigen::Matrix3d constitutiveMatrix() const;

private:
    double E_ = 1.0;
    double nu_ = 0.3;
    ElasticModel2D model_ = ElasticModel2D::PlaneStress;
};

} // namespace OpenCAX

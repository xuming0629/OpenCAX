#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/MeshTypes.h>

#include <functional>
#include <map>

namespace OpenCAX
{

using ScalarFunction2D = std::function<double(double, double)>;

class OpenCAX_API ScalarMaterial2D
{
public:
    ScalarMaterial2D();
    explicit ScalarMaterial2D(double value);
    explicit ScalarMaterial2D(ScalarFunction2D coefficient);

    void setConstant(double value);
    void setCoefficient(ScalarFunction2D coefficient);

    double value(double x, double y) const;

private:
    ScalarFunction2D coefficient_;
};

class OpenCAX_API MaterialManager2D
{
public:
    MaterialManager2D();

    void setDefaultMaterial(const ScalarMaterial2D& material);
    void setDefaultConstant(double value);
    void setDefaultCoefficient(ScalarFunction2D coefficient);

    void setMaterialByMaterialId(int material_id, const ScalarMaterial2D& material);
    void setMaterialByRegionId(int region_id, const ScalarMaterial2D& material);

    bool hasMaterialId(int material_id) const;
    bool hasRegionId(int region_id) const;

    double scalarValue(const MeshCell& cell, double x, double y) const;

private:
    ScalarMaterial2D default_material_;
    std::map<int, ScalarMaterial2D> material_id_map_;
    std::map<int, ScalarMaterial2D> region_id_map_;
};

} // namespace OpenCAX

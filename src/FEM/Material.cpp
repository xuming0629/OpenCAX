#include <OpenCAX/FEM/Material.h>

#include <utility>

namespace OpenCAX
{

ScalarMaterial2D::ScalarMaterial2D()
{
    coefficient_ = [](double, double) { return 1.0; };
}

ScalarMaterial2D::ScalarMaterial2D(double value)
{
    setConstant(value);
}

ScalarMaterial2D::ScalarMaterial2D(ScalarFunction2D coefficient)
    : coefficient_(std::move(coefficient))
{
}

void ScalarMaterial2D::setConstant(double value)
{
    coefficient_ = [value](double, double) { return value; };
}

void ScalarMaterial2D::setCoefficient(ScalarFunction2D coefficient)
{
    coefficient_ = std::move(coefficient);
}

double ScalarMaterial2D::value(double x, double y) const
{
    return coefficient_(x, y);
}

MaterialManager2D::MaterialManager2D()
    : default_material_(1.0)
{
}

void MaterialManager2D::setDefaultMaterial(const ScalarMaterial2D& material)
{
    default_material_ = material;
}

void MaterialManager2D::setDefaultConstant(double value)
{
    default_material_.setConstant(value);
}

void MaterialManager2D::setDefaultCoefficient(ScalarFunction2D coefficient)
{
    default_material_.setCoefficient(std::move(coefficient));
}

void MaterialManager2D::setMaterialByMaterialId(int material_id, const ScalarMaterial2D& material)
{
    material_id_map_[material_id] = material;
}

void MaterialManager2D::setMaterialByRegionId(int region_id, const ScalarMaterial2D& material)
{
    region_id_map_[region_id] = material;
}

bool MaterialManager2D::hasMaterialId(int material_id) const
{
    return material_id_map_.find(material_id) != material_id_map_.end();
}

bool MaterialManager2D::hasRegionId(int region_id) const
{
    return region_id_map_.find(region_id) != region_id_map_.end();
}

double MaterialManager2D::scalarValue(const MeshCell& cell, double x, double y) const
{
    auto it_mat = material_id_map_.find(cell.material_id);
    if (it_mat != material_id_map_.end())
    {
        return it_mat->second.value(x, y);
    }

    auto it_reg = region_id_map_.find(cell.region_id);
    if (it_reg != region_id_map_.end())
    {
        return it_reg->second.value(x, y);
    }

    return default_material_.value(x, y);
}

} // namespace OpenCAX

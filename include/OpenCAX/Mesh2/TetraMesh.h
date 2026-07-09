#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/Mesh.h>

#include <array>
#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API TetraMesh : public Mesh
{
public:
    TetraMesh();

    static TetraMesh create_structured_box(
        double xmin, double xmax,
        double ymin, double ymax,
        double zmin, double zmax,
        int nx, int ny, int nz
    );

    static TetraMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 4>>& tets
    );

    void set_structured_info(const StructuredGridInfo3D& info);
    const StructuredGridInfo3D& structured_info() const;

    static double signed_tetra_volume6(const MeshNode& a, const MeshNode& b, const MeshNode& c, const MeshNode& d);
    static double tetra_volume(const MeshNode& a, const MeshNode& b, const MeshNode& c, const MeshNode& d);

    double volume(int cell_id) const;
    double total_volume() const;
    std::array<double, 3> centroid(int cell_id) const;

    bool validate(std::string* error_message, double volume_eps) const;
    bool validate(std::string* error_message = nullptr) const override;

private:
    StructuredGridInfo3D structured_info_;
};

} // namespace OpenCAX

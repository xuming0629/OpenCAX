#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API HexMesh : public Mesh
{
public:
    HexMesh();

    static HexMesh create_structured_box(
        double xmin, double xmax,
        double ymin, double ymax,
        double zmin, double zmax,
        int nx, int ny, int nz
    );

    static HexMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 8>>& hexes
    );

    void set_structured_info(const StructuredGridInfo3D& info);
    const StructuredGridInfo3D& structured_info() const;

    static double tetra_volume(const MeshNode& a, const MeshNode& b, const MeshNode& c, const MeshNode& d);
    static double hexa_volume(const MeshNode& a, const MeshNode& b, const MeshNode& c, const MeshNode& d,
                              const MeshNode& e, const MeshNode& f, const MeshNode& g, const MeshNode& h);

    double volume(int cell_id) const;
    double total_volume() const;
    std::array<double, 3> centroid(int cell_id) const;

    bool validate(std::string* error_message, double volume_eps) const;
    bool validate(std::string* error_message = nullptr) const override;

private:
    StructuredGridInfo3D structured_info_;
};

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/Mesh.h>

#include <array>
#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API QuadMesh : public Mesh
{
public:
    QuadMesh();

    static QuadMesh create_structured_rectangle(
        double xmin, double xmax,
        double ymin, double ymax,
        int nx, int ny
    );

    static QuadMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 4>>& quads
    );

    void set_structured_info(const StructuredGridInfo2D& info);
    const StructuredGridInfo2D& structured_info() const;

    static double triangle_area(const MeshNode& a, const MeshNode& b, const MeshNode& c);
    static double quad_area(const MeshNode& a, const MeshNode& b, const MeshNode& c, const MeshNode& d);

    double area(int cell_id) const;
    double total_area() const;
    std::array<double, 3> centroid(int cell_id) const;

    bool validate(std::string* error_message, double area_eps) const;
    bool validate(std::string* error_message = nullptr) const override;

private:
    StructuredGridInfo2D structured_info_;
};

} // namespace OpenCAX

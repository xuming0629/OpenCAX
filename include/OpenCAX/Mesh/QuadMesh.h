#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

enum class QuadMeshSourceType
{
    Unknown = 0,
    Structured,
    Unstructured
};

struct QuadMeshStructuredInfo
{
    int nx = 0;
    int ny = 0;

    double xmin = 0.0;
    double xmax = 0.0;
    double ymin = 0.0;
    double ymax = 0.0;
};

class OpenCAX_API QuadMesh : public Mesh
{
public:
    QuadMesh();

    static QuadMesh create_structured_rectangle(
        double xmin,
        double xmax,
        double ymin,
        double ymax,
        int nx,
        int ny
    );

    static QuadMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 4>>& quads
    );

    QuadMeshSourceType source_type() const;
    void set_source_type(QuadMeshSourceType type);

    bool is_structured() const;
    bool is_unstructured() const;

    const QuadMeshStructuredInfo& structured_info() const;

    double area(int cell_id) const;
    double total_area() const;
    std::array<double, 3> centroid(int cell_id) const;

    std::array<double, 6> bounds() const;

    bool validate(
        std::string* error_message = nullptr,
        double area_eps = 1e-14
    ) const;

private:
    static double triangle_area(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

    static double quad_area(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c,
        const MeshNode& d
    );

private:
    QuadMeshSourceType source_type_ = QuadMeshSourceType::Unknown;
    QuadMeshStructuredInfo structured_info_;
};

} // namespace OpenCAX
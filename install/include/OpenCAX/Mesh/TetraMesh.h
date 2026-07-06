#pragma once

/**
 * @file TetraMesh.h
 * @brief OpenCAX 三维四面体网格类声明
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

enum class TetraMeshSourceType
{
    Unknown = 0,
    Structured,
    Unstructured
};

struct TetraMeshStructuredInfo
{
    int nx = 0;
    int ny = 0;
    int nz = 0;

    double xmin = 0.0;
    double xmax = 0.0;

    double ymin = 0.0;
    double ymax = 0.0;

    double zmin = 0.0;
    double zmax = 0.0;
};

class OpenCAX_API TetraMesh : public Mesh
{
public:
    TetraMesh();

    static TetraMesh create_structured_box(
        double xmin,
        double xmax,
        double ymin,
        double ymax,
        double zmin,
        double zmax,
        int nx,
        int ny,
        int nz
    );

    static TetraMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 4>>& tets
    );

    TetraMeshSourceType source_type() const;
    void set_source_type(TetraMeshSourceType type);

    bool is_structured() const;
    bool is_unstructured() const;

    const TetraMeshStructuredInfo& structured_info() const;

    double volume(int cell_id) const;
    double total_volume() const;

    std::array<double, 3> centroid(int cell_id) const;

    std::array<double, 6> bounds() const;

    bool validate(
        std::string* error_message = nullptr,
        double volume_eps = 1e-14
    ) const;

private:
    static double tetra_volume(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c,
        const MeshNode& d
    );

private:
    TetraMeshSourceType source_type_ = TetraMeshSourceType::Unknown;
    TetraMeshStructuredInfo structured_info_;
};

} // namespace OpenCAX
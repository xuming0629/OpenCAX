#pragma once

/**
 * @file HexMesh.h
 * @brief OpenCAX 三维六面体网格类声明
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

enum class HexMeshSourceType
{
    Unknown = 0,
    Structured,
    Unstructured
};

struct HexMeshStructuredInfo
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

class OpenCAX_API HexMesh : public Mesh
{
public:
    HexMesh();

    static HexMesh create_structured_box(
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

    static HexMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 8>>& hexes
    );

    HexMeshSourceType source_type() const;
    void set_source_type(HexMeshSourceType type);

    bool is_structured() const;
    bool is_unstructured() const;

    const HexMeshStructuredInfo& structured_info() const;

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

    static double hexa_volume(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c,
        const MeshNode& d,
        const MeshNode& e,
        const MeshNode& f,
        const MeshNode& g,
        const MeshNode& h
    );

private:
    HexMeshSourceType source_type_ = HexMeshSourceType::Unknown;
    HexMeshStructuredInfo structured_info_;
};

} // namespace OpenCAX
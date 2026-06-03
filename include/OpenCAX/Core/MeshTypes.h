#pragma once

#include <vector>
#include <string>

namespace OpenCAX
{

enum class TopologyDim
{
    Dim0 = 0,
    Dim1 = 1,
    Dim2 = 2,
    Dim3 = 3
};

enum class GeometryDim
{
    Dim1 = 1,
    Dim2 = 2,
    Dim3 = 3
};

enum class MeshKind
{
    Unknown = 0,
    Curve,
    Plane,
    Surface,
    Volume
};

enum class MeshStructure
{
    Unknown = 0,
    Structured,
    Unstructured,
    Hybrid
};

enum class CellType
{
    Unknown = 0,

    Point1,

    Line2,
    Line3,

    Triangle3,
    Triangle6,

    Quad4,
    Quad8,
    Quad9,

    Polygon,

    Tetra4,
    Tetra10,

    Hexa8,
    Hexa20,
    Hexa27,

    Prism6,
    Pyramid5
};

struct MeshNode
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct MeshCell
{
    CellType type = CellType::Unknown;
    std::vector<int> node_ids;

    int physical_id = -1;
    int geometric_id = -1;
};

struct MeshInfo
{
    TopologyDim topology_dim = TopologyDim::Dim3;
    GeometryDim geometry_dim = GeometryDim::Dim3;

    MeshKind kind = MeshKind::Unknown;
    MeshStructure structure = MeshStructure::Unknown;

    bool is_second_order = false;
};

}
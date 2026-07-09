#pragma once

#include <string>
#include <vector>

namespace OpenCAX
{

enum class MeshDimension
{
    Unknown = -1,
    Dim0 = 0,
    Dim1 = 1,
    Dim2 = 2,
    Dim3 = 3
};

enum class CellType
{
    Unknown = 0,

    Vertex1,

    Line2,
    Line3,

    Triangle3,
    Triangle6,

    Quad4,
    Quad8,
    Quad9,

    Tetra4,
    Tetra10,

    Hexa8,
    Hexa20,
    Hexa27,

    Prism6,
    Prism15,

    Pyramid5,
    Pyramid13
};

enum class MeshOrder
{
    Unknown = 0,
    Linear = 1,
    Quadratic = 2
};

enum class MeshElementFamily
{
    Unknown,
    Vertex,
    Line,
    Triangle,
    Quadrilateral,
    Tetrahedron,
    Hexahedron,
    Prism,
    Pyramid,
    Mixed
};

enum class MeshSourceType
{
    Unknown,
    Structured,
    Unstructured,
    Gmsh,
    TetGen,
    Imported,
    Refined,
    HighOrderConverted
};

struct MeshNode
{
    int id = -1;

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    int physical_id = -1;
    int boundary_id = -1;
    int geometry_id = -1;
};

struct MeshCell
{
    int id = -1;
    CellType type = CellType::Unknown;
    std::vector<int> node_ids;

    int order = 0;

    int physical_id = -1;
    int material_id = -1;
    int region_id = -1;
    int geometry_id = -1;
};

struct MeshInfo
{
    std::string name = "OpenCAXMesh";
    std::string source = "OpenCAX";

    MeshDimension dimension = MeshDimension::Unknown;
    MeshSourceType source_type = MeshSourceType::Unknown;

    bool structured = false;
    bool high_order = false;
};

struct StructuredGridInfo2D
{
    int nx = 0;
    int ny = 0;

    double xmin = 0.0;
    double xmax = 0.0;
    double ymin = 0.0;
    double ymax = 0.0;

    bool alternate_diagonal = false;
};

struct StructuredGridInfo3D
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

const char* to_string(CellType type);
const char* to_string(MeshDimension dimension);
const char* to_string(MeshSourceType source_type);

} // namespace OpenCAX

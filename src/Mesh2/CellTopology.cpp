#include <OpenCAX/Mesh2/CellTopology.h>

namespace OpenCAX
{

std::array<int, 2> CellTopology::edge(int a, int b) { return {a, b}; }
std::vector<int> CellTopology::face(std::initializer_list<int> ids) { return std::vector<int>(ids); }

MeshDimension CellTopology::dimension(CellType type)
{
    switch (type)
    {
    case CellType::Vertex1: return MeshDimension::Dim0;
    case CellType::Line2:
    case CellType::Line3: return MeshDimension::Dim1;
    case CellType::Triangle3:
    case CellType::Triangle6:
    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9: return MeshDimension::Dim2;
    case CellType::Tetra4:
    case CellType::Tetra10:
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
    case CellType::Prism6:
    case CellType::Prism15:
    case CellType::Pyramid5:
    case CellType::Pyramid13: return MeshDimension::Dim3;
    default: return MeshDimension::Unknown;
    }
}

int CellTopology::order(CellType type)
{
    switch (type)
    {
    case CellType::Line3:
    case CellType::Triangle6:
    case CellType::Quad8:
    case CellType::Quad9:
    case CellType::Tetra10:
    case CellType::Hexa20:
    case CellType::Hexa27:
    case CellType::Prism15:
    case CellType::Pyramid13: return 2;
    case CellType::Vertex1:
    case CellType::Line2:
    case CellType::Triangle3:
    case CellType::Quad4:
    case CellType::Tetra4:
    case CellType::Hexa8:
    case CellType::Prism6:
    case CellType::Pyramid5: return 1;
    default: return 0;
    }
}

int CellTopology::num_nodes(CellType type)
{
    switch (type)
    {
    case CellType::Vertex1: return 1;
    case CellType::Line2: return 2;
    case CellType::Line3: return 3;
    case CellType::Triangle3: return 3;
    case CellType::Triangle6: return 6;
    case CellType::Quad4: return 4;
    case CellType::Quad8: return 8;
    case CellType::Quad9: return 9;
    case CellType::Tetra4: return 4;
    case CellType::Tetra10: return 10;
    case CellType::Hexa8: return 8;
    case CellType::Hexa20: return 20;
    case CellType::Hexa27: return 27;
    case CellType::Prism6: return 6;
    case CellType::Prism15: return 15;
    case CellType::Pyramid5: return 5;
    case CellType::Pyramid13: return 13;
    default: return 0;
    }
}

int CellTopology::num_vertices(CellType type)
{
    switch (type)
    {
    case CellType::Vertex1: return 1;
    case CellType::Line2:
    case CellType::Line3: return 2;
    case CellType::Triangle3:
    case CellType::Triangle6: return 3;
    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9:
    case CellType::Tetra4:
    case CellType::Tetra10: return 4;
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27: return 8;
    case CellType::Prism6:
    case CellType::Prism15: return 6;
    case CellType::Pyramid5:
    case CellType::Pyramid13: return 5;
    default: return 0;
    }
}

int CellTopology::num_edges(CellType type)
{
    switch (type)
    {
    case CellType::Line2:
    case CellType::Line3: return 1;
    case CellType::Triangle3:
    case CellType::Triangle6: return 3;
    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9: return 4;
    case CellType::Tetra4:
    case CellType::Tetra10: return 6;
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27: return 12;
    case CellType::Prism6:
    case CellType::Prism15: return 9;
    case CellType::Pyramid5:
    case CellType::Pyramid13: return 8;
    default: return 0;
    }
}

int CellTopology::num_faces(CellType type)
{
    switch (type)
    {
    case CellType::Tetra4:
    case CellType::Tetra10: return 4;
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27: return 6;
    case CellType::Prism6:
    case CellType::Prism15: return 5;
    case CellType::Pyramid5:
    case CellType::Pyramid13: return 5;
    default: return 0;
    }
}

bool CellTopology::is_high_order(CellType type) { return order(type) > 1; }
bool CellTopology::is_line_cell(CellType type) { return dimension(type) == MeshDimension::Dim1; }
bool CellTopology::is_surface_cell(CellType type) { return dimension(type) == MeshDimension::Dim2; }
bool CellTopology::is_volume_cell(CellType type) { return dimension(type) == MeshDimension::Dim3; }
bool CellTopology::is_simplex(CellType type) { return family(type) == MeshElementFamily::Triangle || family(type) == MeshElementFamily::Tetrahedron; }
bool CellTopology::is_tensor_product(CellType type) { return family(type) == MeshElementFamily::Quadrilateral || family(type) == MeshElementFamily::Hexahedron; }

MeshElementFamily CellTopology::family(CellType type)
{
    switch (type)
    {
    case CellType::Vertex1: return MeshElementFamily::Vertex;
    case CellType::Line2:
    case CellType::Line3: return MeshElementFamily::Line;
    case CellType::Triangle3:
    case CellType::Triangle6: return MeshElementFamily::Triangle;
    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9: return MeshElementFamily::Quadrilateral;
    case CellType::Tetra4:
    case CellType::Tetra10: return MeshElementFamily::Tetrahedron;
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27: return MeshElementFamily::Hexahedron;
    case CellType::Prism6:
    case CellType::Prism15: return MeshElementFamily::Prism;
    case CellType::Pyramid5:
    case CellType::Pyramid13: return MeshElementFamily::Pyramid;
    default: return MeshElementFamily::Unknown;
    }
}

const char* CellTopology::to_string(CellType type)
{
    switch(type)
    {
    case CellType::Vertex1: return "Vertex1";
    case CellType::Line2: return "Line2";
    case CellType::Line3: return "Line3";
    case CellType::Triangle3: return "Triangle3";
    case CellType::Triangle6: return "Triangle6";
    case CellType::Quad4: return "Quad4";
    case CellType::Quad8: return "Quad8";
    case CellType::Quad9: return "Quad9";
    case CellType::Tetra4: return "Tetra4";
    case CellType::Tetra10: return "Tetra10";
    case CellType::Hexa8: return "Hexa8";
    case CellType::Hexa20: return "Hexa20";
    case CellType::Hexa27: return "Hexa27";
    case CellType::Prism6: return "Prism6";
    case CellType::Prism15: return "Prism15";
    case CellType::Pyramid5: return "Pyramid5";
    case CellType::Pyramid13: return "Pyramid13";
    default: return "Unknown";
    }
}

std::vector<int> CellTopology::vertex_indices(CellType type)
{
    const int n = num_vertices(type);
    std::vector<int> ids;
    for (int i = 0; i < n; ++i) ids.push_back(i);
    return ids;
}

std::vector<std::array<int,2>> CellTopology::topology_edges(CellType type, const std::vector<int>& n)
{
    switch (type)
    {
    case CellType::Line2:
    case CellType::Line3:
        if (n.size() >= 2) return {edge(n[0], n[1])};
        break;
    case CellType::Triangle3:
    case CellType::Triangle6:
        if (n.size() >= 3) return {edge(n[0],n[1]), edge(n[1],n[2]), edge(n[2],n[0])};
        break;
    case CellType::Quad4:
    case CellType::Quad8:
    case CellType::Quad9:
        if (n.size() >= 4) return {edge(n[0],n[1]), edge(n[1],n[2]), edge(n[2],n[3]), edge(n[3],n[0])};
        break;
    case CellType::Tetra4:
    case CellType::Tetra10:
        if (n.size() >= 4) return {edge(n[0],n[1]), edge(n[1],n[2]), edge(n[2],n[0]), edge(n[0],n[3]), edge(n[1],n[3]), edge(n[2],n[3])};
        break;
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        if (n.size() >= 8) return {edge(n[0],n[1]),edge(n[1],n[2]),edge(n[2],n[3]),edge(n[3],n[0]),edge(n[4],n[5]),edge(n[5],n[6]),edge(n[6],n[7]),edge(n[7],n[4]),edge(n[0],n[4]),edge(n[1],n[5]),edge(n[2],n[6]),edge(n[3],n[7])};
        break;
    case CellType::Prism6:
    case CellType::Prism15:
        if (n.size() >= 6) return {edge(n[0],n[1]),edge(n[1],n[2]),edge(n[2],n[0]),edge(n[3],n[4]),edge(n[4],n[5]),edge(n[5],n[3]),edge(n[0],n[3]),edge(n[1],n[4]),edge(n[2],n[5])};
        break;
    case CellType::Pyramid5:
    case CellType::Pyramid13:
        if (n.size() >= 5) return {edge(n[0],n[1]),edge(n[1],n[2]),edge(n[2],n[3]),edge(n[3],n[0]),edge(n[0],n[4]),edge(n[1],n[4]),edge(n[2],n[4]),edge(n[3],n[4])};
        break;
    default: break;
    }
    return {};
}

std::vector<std::array<int,2>> CellTopology::visual_edges(CellType type, const std::vector<int>& n)
{
    switch (type)
    {
    case CellType::Line3:
        if (n.size() >= 3) return {edge(n[0],n[2]), edge(n[2],n[1])};
        break;
    case CellType::Triangle6:
        if (n.size() >= 6) return {edge(n[0],n[3]), edge(n[3],n[1]), edge(n[1],n[4]), edge(n[4],n[2]), edge(n[2],n[5]), edge(n[5],n[0])};
        break;
    case CellType::Quad8:
    case CellType::Quad9:
        if (n.size() >= 8) return {edge(n[0],n[4]),edge(n[4],n[1]),edge(n[1],n[5]),edge(n[5],n[2]),edge(n[2],n[6]),edge(n[6],n[3]),edge(n[3],n[7]),edge(n[7],n[0])};
        break;
    case CellType::Tetra10:
        if (n.size() >= 10) return {edge(n[0],n[4]),edge(n[4],n[1]),edge(n[1],n[5]),edge(n[5],n[2]),edge(n[2],n[6]),edge(n[6],n[0]),edge(n[0],n[7]),edge(n[7],n[3]),edge(n[1],n[8]),edge(n[8],n[3]),edge(n[2],n[9]),edge(n[9],n[3])};
        break;
    default: break;
    }
    return topology_edges(type, n);
}

std::vector<std::array<int,2>> CellTopology::edges(CellType type, const std::vector<int>& node_ids)
{
    return topology_edges(type, node_ids);
}

std::vector<std::vector<int>> CellTopology::faces(CellType type, const std::vector<int>& n)
{
    switch (type)
    {
    case CellType::Tetra4:
    case CellType::Tetra10:
        if (n.size() >= 4) return {face({n[0],n[2],n[1]}), face({n[0],n[1],n[3]}), face({n[1],n[2],n[3]}), face({n[2],n[0],n[3]})};
        break;
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27:
        if (n.size() >= 8) return {face({n[0],n[1],n[2],n[3]}), face({n[4],n[5],n[6],n[7]}), face({n[0],n[1],n[5],n[4]}), face({n[1],n[2],n[6],n[5]}), face({n[2],n[3],n[7],n[6]}), face({n[3],n[0],n[4],n[7]})};
        break;
    case CellType::Prism6:
    case CellType::Prism15:
        if (n.size() >= 6) return {face({n[0],n[2],n[1]}), face({n[3],n[4],n[5]}), face({n[0],n[1],n[4],n[3]}), face({n[1],n[2],n[5],n[4]}), face({n[2],n[0],n[3],n[5]})};
        break;
    case CellType::Pyramid5:
    case CellType::Pyramid13:
        if (n.size() >= 5) return {face({n[0],n[1],n[2],n[3]}), face({n[0],n[1],n[4]}), face({n[1],n[2],n[4]}), face({n[2],n[3],n[4]}), face({n[3],n[0],n[4]})};
        break;
    default: break;
    }
    return {};
}

std::vector<CellType> CellTopology::face_types(CellType type)
{
    switch (type)
    {
    case CellType::Tetra4:
    case CellType::Tetra10: return {CellType::Triangle3, CellType::Triangle3, CellType::Triangle3, CellType::Triangle3};
    case CellType::Hexa8:
    case CellType::Hexa20:
    case CellType::Hexa27: return {CellType::Quad4, CellType::Quad4, CellType::Quad4, CellType::Quad4, CellType::Quad4, CellType::Quad4};
    case CellType::Prism6:
    case CellType::Prism15: return {CellType::Triangle3, CellType::Triangle3, CellType::Quad4, CellType::Quad4, CellType::Quad4};
    case CellType::Pyramid5:
    case CellType::Pyramid13: return {CellType::Quad4, CellType::Triangle3, CellType::Triangle3, CellType::Triangle3, CellType::Triangle3};
    default: return {};
    }
}

} // namespace OpenCAX

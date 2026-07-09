#include <OpenCAX/Meshing/Gmsh/GmshElementMap.h>

namespace OpenCAX
{

CellType GmshElementMap::to_opencax(int t)
{
    switch(t)
    {
    case 1: return CellType::Line2;
    case 2: return CellType::Triangle3;
    case 3: return CellType::Quad4;
    case 4: return CellType::Tetra4;
    case 5: return CellType::Hexa8;
    case 6: return CellType::Prism6;
    case 7: return CellType::Pyramid5;
    case 8: return CellType::Line3;
    case 9: return CellType::Triangle6;
    case 10: return CellType::Quad9;
    case 11: return CellType::Tetra10;
    case 12: return CellType::Hexa27;
    case 16: return CellType::Quad8;
    case 17: return CellType::Hexa20;
    case 18: return CellType::Prism15;
    case 19: return CellType::Pyramid13;
    default: return CellType::Unknown;
    }
}

int GmshElementMap::to_gmsh(CellType t)
{
    switch(t)
    {
    case CellType::Line2: return 1;
    case CellType::Triangle3: return 2;
    case CellType::Quad4: return 3;
    case CellType::Tetra4: return 4;
    case CellType::Hexa8: return 5;
    case CellType::Prism6: return 6;
    case CellType::Pyramid5: return 7;
    case CellType::Line3: return 8;
    case CellType::Triangle6: return 9;
    case CellType::Quad9: return 10;
    case CellType::Tetra10: return 11;
    case CellType::Hexa27: return 12;
    case CellType::Quad8: return 16;
    case CellType::Hexa20: return 17;
    case CellType::Prism15: return 18;
    case CellType::Pyramid13: return 19;
    default: return 0;
    }
}

std::vector<int> GmshElementMap::reorder_to_opencax(CellType, const std::vector<int>& ids)
{
    // OpenCAX currently adopts Gmsh-compatible ordering for common first/second-order cells.
    return ids;
}

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Mesh2/MeshTypes.h>
#include <vector>

namespace OpenCAX
{

class GmshElementMap
{
public:
    static CellType to_opencax(int gmsh_type);
    static int to_gmsh(CellType type);
    static std::vector<int> reorder_to_opencax(CellType type, const std::vector<int>& ids);
};

} // namespace OpenCAX

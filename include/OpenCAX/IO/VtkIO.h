#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/MeshTypes.h>

#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API VtkIO
{
public:
    static bool writeVTU(
        const std::vector<MeshNode>& nodes,
        const std::vector<std::vector<int>>& cells,
        const std::vector<CellType>& cell_types,
        const std::string& filename,
        bool binary = true
    );
};

} // namespace OpenCAX
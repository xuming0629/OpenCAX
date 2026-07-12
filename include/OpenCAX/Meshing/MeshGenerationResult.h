#pragma once

#include <OpenCAX/Mesh/Mesh.h>
#include <memory>
#include <string>
#include <vector>

namespace OpenCAX
{

struct MeshGenerationResult
{
    bool success = false;
    std::shared_ptr<Mesh> mesh;
    std::string message;
    double generation_time = 0.0;
    std::vector<int> physical_ids;
    std::vector<std::string> physical_names;
};

} // namespace OpenCAX

#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Meshing/MeshGenerationOptions.h>
#include <OpenCAX/Meshing/MeshGenerationResult.h>

#include <string>

namespace OpenCAX
{

class OpenCAX_API MeshGenerator
{
public:
    virtual ~MeshGenerator() = default;
    virtual std::string name() const = 0;
    virtual MeshGenerationResult generate(const MeshGenerationOptions& options) = 0;
};

} // namespace OpenCAX

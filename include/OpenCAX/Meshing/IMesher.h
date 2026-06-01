#pragma once

#include <OpenCAX/Meshing/Mesh.h>
#include <OpenCAX/Meshing/MeshingOptions.h>

namespace OpenCAX
{

class IMesher
{
public:
    virtual ~IMesher() = default;

    virtual Mesh generate(const MeshingOptions& options) = 0;
};

}
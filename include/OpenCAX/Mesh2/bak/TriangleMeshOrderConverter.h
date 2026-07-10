#pragma once

#include <OpenCAX/Meshing/HighOrder/MeshOrderConverter.h>

namespace OpenCAX
{

/** Backward-compatible wrapper. New code should use OpenCAX/Meshing/HighOrder/MeshOrderConverter.h. */
class TriangleMeshOrderConverter
{
public:
    static TriangleMesh to_quadratic(const TriangleMesh& mesh)
    {
        return MeshOrderConverter::triangle3_to_triangle6(mesh);
    }
};

} // namespace OpenCAX

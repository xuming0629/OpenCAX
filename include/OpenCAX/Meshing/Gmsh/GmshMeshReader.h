#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>
#include <OpenCAX/Mesh/TriangleMesh.h>
#include <OpenCAX/Mesh/TetraMesh.h>

#include <string>

namespace OpenCAX
{

class OpenCAX_API GmshMeshReader
{
public:
    static Mesh read_mesh(const std::string& filename);
    static TriangleMesh read_triangle_mesh(const std::string& filename);
    static TetraMesh read_tetra_mesh(const std::string& filename);
};

} // namespace OpenCAX

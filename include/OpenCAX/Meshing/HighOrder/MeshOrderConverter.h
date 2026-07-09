#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/TriangleMesh.h>
#include <OpenCAX/Mesh2/QuadMesh.h>
#include <OpenCAX/Mesh2/TetraMesh.h>
#include <OpenCAX/Mesh2/HexMesh.h>

namespace OpenCAX
{

class OpenCAX_API MeshOrderConverter
{
public:
    static TriangleMesh triangle3_to_triangle6(const TriangleMesh& mesh);
    static TriangleMesh to_quadratic(const TriangleMesh& mesh);

    static QuadMesh quad4_to_quad8(const QuadMesh& mesh);
    static QuadMesh quad4_to_quad9(const QuadMesh& mesh);

    static TetraMesh tetra4_to_tetra10(const TetraMesh& mesh);

    static HexMesh hexa8_to_hexa20(const HexMesh& mesh);
    static HexMesh hexa8_to_hexa27(const HexMesh& mesh);
};

} // namespace OpenCAX

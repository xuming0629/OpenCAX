#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <vector>

namespace OpenCAX
{

class OpenCAX_API MeshRefiner
{
public:
    static TriangleMesh uniform_refine(const TriangleMesh& mesh, int levels = 1);
    static TriangleMesh bisection_refine(const TriangleMesh& mesh, int levels = 1);
    static TriangleMesh bisection_refine_marked_cells(const TriangleMesh& mesh, const std::vector<int>& marked_cell_ids);
};

} // namespace OpenCAX

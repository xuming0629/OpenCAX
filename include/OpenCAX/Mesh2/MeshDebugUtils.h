#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/Mesh.h>
#include <OpenCAX/Mesh2/MeshTopology.h>

#include <iosfwd>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API MeshDebugUtils
{
public:
    static void print_vector_int(const std::vector<int>& values, std::ostream& os);
    static void print_vector_int(const std::vector<int>& values);

    static void print_mesh(const Mesh& mesh, std::ostream& os);
    static void print_mesh(const Mesh& mesh);

    static void print_topology(const MeshTopology& topo, std::ostream& os);
    static void print_topology(const MeshTopology& topo);

    static bool check_mesh(const Mesh& mesh, std::ostream& os);
    static bool check_mesh(const Mesh& mesh);
};

} // namespace OpenCAX

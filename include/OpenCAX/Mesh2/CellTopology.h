#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/MeshTypes.h>

#include <array>
#include <initializer_list>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API CellTopology
{
public:
    static MeshDimension dimension(CellType type);
    static int order(CellType type);
    static int num_nodes(CellType type);
    static int num_vertices(CellType type);
    static int num_edges(CellType type);
    static int num_faces(CellType type);

    static bool is_high_order(CellType type);
    static bool is_line_cell(CellType type);
    static bool is_surface_cell(CellType type);
    static bool is_volume_cell(CellType type);
    static bool is_simplex(CellType type);
    static bool is_tensor_product(CellType type);

    static MeshElementFamily family(CellType type);
    static const char* to_string(CellType type);

    static std::vector<int> vertex_indices(CellType type);

    /** Mathematical/topological edges: used by MeshTopology, FEM adjacency. */
    static std::vector<std::array<int, 2>> topology_edges(
        CellType type,
        const std::vector<int>& node_ids
    );

    /** Visual edges: split high-order edges through mid-side nodes for drawing. */
    static std::vector<std::array<int, 2>> visual_edges(
        CellType type,
        const std::vector<int>& node_ids
    );

    /** Backward compatible alias: topology edges. */
    static std::vector<std::array<int, 2>> edges(
        CellType type,
        const std::vector<int>& node_ids
    );

    static std::vector<std::vector<int>> faces(
        CellType type,
        const std::vector<int>& node_ids
    );

    static std::vector<CellType> face_types(CellType type);

private:
    static std::array<int, 2> edge(int a, int b);
    static std::vector<int> face(std::initializer_list<int> ids);
};

} // namespace OpenCAX

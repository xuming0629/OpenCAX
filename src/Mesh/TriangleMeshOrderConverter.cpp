#include <OpenCAX/Mesh/TriangleMeshOrderConverter.h>

#include <OpenCAX/Mesh/MeshTypes.h>

#include <array>
#include <map>
#include <stdexcept>
#include <utility>

namespace OpenCAX
{

namespace
{

static std::array<int, 2> make_edge_key(
    int a,
    int b
)
{
    if (a < b)
    {
        return {a, b};
    }

    return {b, a};
}

static int get_or_create_mid_node(
    TriangleMesh& out,
    const TriangleMesh& in,
    std::map<std::array<int, 2>, int>& edge_mid_node,
    int a,
    int b
)
{
    const std::array<int, 2> key =
        make_edge_key(
            a,
            b
        );

    auto it =
        edge_mid_node.find(key);

    if (it != edge_mid_node.end())
    {
        return it->second;
    }

    const auto& nodes =
        in.nodes();

    const MeshNode& na =
        nodes[static_cast<std::size_t>(a)];

    const MeshNode& nb =
        nodes[static_cast<std::size_t>(b)];

    const double mx =
        0.5 * (na.x + nb.x);

    const double my =
        0.5 * (na.y + nb.y);

    const double mz =
        0.5 * (na.z + nb.z);

    const int mid_id =
        out.add_node(
            mx,
            my,
            mz
        );

    edge_mid_node[key] =
        mid_id;

    return mid_id;
}

} // namespace

TriangleMesh TriangleMeshOrderConverter::to_quadratic(
    const TriangleMesh& mesh
)
{
    TriangleMesh out;

    /*
     * 1. 复制原始节点
     */
    for (const auto& node : mesh.nodes())
    {
        out.add_node(
            node.x,
            node.y,
            node.z,
            node.physical_id,
            node.boundary_id
        );
    }

    /*
     * 2. 每条边创建一个唯一中点
     */
    std::map<std::array<int, 2>, int> edge_mid_node;

    for (const auto& cell : mesh.cells())
    {
        if (cell.type != CellType::Triangle3)
        {
            throw std::runtime_error(
                "[OpenCAX::TriangleMeshOrderConverter] "
                "only Triangle3 cells can be converted to Triangle6."
            );
        }

        if (cell.node_ids.size() != 3)
        {
            throw std::runtime_error(
                "[OpenCAX::TriangleMeshOrderConverter] "
                "Triangle3 cell must have 3 nodes."
            );
        }

        const int v0 =
            cell.node_ids[0];

        const int v1 =
            cell.node_ids[1];

        const int v2 =
            cell.node_ids[2];

        const int m01 =
            get_or_create_mid_node(
                out,
                mesh,
                edge_mid_node,
                v0,
                v1
            );

        const int m12 =
            get_or_create_mid_node(
                out,
                mesh,
                edge_mid_node,
                v1,
                v2
            );

        const int m20 =
            get_or_create_mid_node(
                out,
                mesh,
                edge_mid_node,
                v2,
                v0
            );

        const int qcell_id =
            out.add_cell(
                CellType::Triangle6,
                {
                    v0,
                    v1,
                    v2,
                    m01,
                    m12,
                    m20
                },
                cell.physical_id,
                cell.material_id,
                cell.region_id
            );

        out.cells()[static_cast<std::size_t>(qcell_id)].order = 2;
    }

    out.info().dimension = mesh.info().dimension;
    out.info().name = mesh.info().name + "_quadratic";
    out.info().source = mesh.info().source;

    return out;
}

} // namespace OpenCAX
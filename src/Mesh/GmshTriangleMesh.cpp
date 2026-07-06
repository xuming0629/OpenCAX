#include <OpenCAX/Mesh//GmshTriangleMesh.h>

#include <gmsh.h>

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace OpenCAX
{

namespace
{

void ensure_gmsh_initialized(
    bool verbose
)
{
    if (!gmsh::isInitialized())
    {
        gmsh::initialize();
    }

    gmsh::option::setNumber(
        "General.Terminal",
        verbose ? 1.0 : 0.0
    );
}

void check_rectangle_args(
    double xmin,
    double xmax,
    double ymin,
    double ymax
)
{
    if (!(xmax > xmin))
    {
        throw std::invalid_argument(
            "GmshTriangleMeshGenerator: xmax must be greater than xmin."
        );
    }

    if (!(ymax > ymin))
    {
        throw std::invalid_argument(
            "GmshTriangleMeshGenerator: ymax must be greater than ymin."
        );
    }
}

void check_structured_args(
    int nx,
    int ny
)
{
    if (nx <= 0)
    {
        throw std::invalid_argument(
            "GmshTriangleMeshGenerator: nx must be positive."
        );
    }

    if (ny <= 0)
    {
        throw std::invalid_argument(
            "GmshTriangleMeshGenerator: ny must be positive."
        );
    }
}

/**
 * @brief 创建 Gmsh 矩形几何
 */
int create_rectangle_geometry(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    double mesh_size,
    int& p1,
    int& p2,
    int& p3,
    int& p4,
    int& l1,
    int& l2,
    int& l3,
    int& l4
)
{
    p1 = gmsh::model::geo::addPoint(
        xmin,
        ymin,
        0.0,
        mesh_size
    );

    p2 = gmsh::model::geo::addPoint(
        xmax,
        ymin,
        0.0,
        mesh_size
    );

    p3 = gmsh::model::geo::addPoint(
        xmax,
        ymax,
        0.0,
        mesh_size
    );

    p4 = gmsh::model::geo::addPoint(
        xmin,
        ymax,
        0.0,
        mesh_size
    );

    l1 = gmsh::model::geo::addLine(p1, p2);
    l2 = gmsh::model::geo::addLine(p2, p3);
    l3 = gmsh::model::geo::addLine(p3, p4);
    l4 = gmsh::model::geo::addLine(p4, p1);

    const int loop = gmsh::model::geo::addCurveLoop({
        l1,
        l2,
        l3,
        l4
    });

    const int surface = gmsh::model::geo::addPlaneSurface({
        loop
    });

    return surface;
}

/**
 * @brief 从当前 Gmsh 模型读取一阶三角形网格
 */
TriangleMesh read_triangle_mesh_from_gmsh()
{
    std::vector<std::size_t> node_tags;
    std::vector<double> node_coords;
    std::vector<double> node_params;

    gmsh::model::mesh::getNodes(
        node_tags,
        node_coords,
        node_params
    );

    if (node_tags.empty())
    {
        throw std::runtime_error(
            "GmshTriangleMeshGenerator: gmsh generated no nodes."
        );
    }

    if (node_coords.size() != node_tags.size() * 3)
    {
        throw std::runtime_error(
            "GmshTriangleMeshGenerator: invalid node coordinate array."
        );
    }

    std::vector<std::array<double, 3>> points;
    points.reserve(node_tags.size());

    std::unordered_map<std::size_t, int> tag_to_index;
    tag_to_index.reserve(node_tags.size());

    for (std::size_t i = 0; i < node_tags.size(); ++i)
    {
        const std::size_t tag = node_tags[i];

        const double x = node_coords[3 * i + 0];
        const double y = node_coords[3 * i + 1];
        const double z = node_coords[3 * i + 2];

        const int local_id =
            static_cast<int>(points.size());

        tag_to_index[tag] = local_id;

        points.push_back({
            x,
            y,
            z
        });
    }

    std::vector<int> element_types;
    std::vector<std::vector<std::size_t>> element_tags;
    std::vector<std::vector<std::size_t>> element_node_tags;

    gmsh::model::mesh::getElements(
        element_types,
        element_tags,
        element_node_tags,
        2
    );

    std::vector<std::array<int, 3>> triangles;

    for (std::size_t block_id = 0;
         block_id < element_types.size();
         ++block_id)
    {
        const int element_type =
            element_types[block_id];

        /*
         * Gmsh element type:
         *
         * 2 = 3-node triangle
         *
         * 当前只读取一阶 Triangle3。
         */
        if (element_type != 2)
        {
            continue;
        }

        const auto& nodes =
            element_node_tags[block_id];

        if (nodes.size() % 3 != 0)
        {
            throw std::runtime_error(
                "GmshTriangleMeshGenerator: invalid Triangle3 connectivity."
            );
        }

        for (std::size_t i = 0; i < nodes.size(); i += 3)
        {
            const std::size_t t0 = nodes[i + 0];
            const std::size_t t1 = nodes[i + 1];
            const std::size_t t2 = nodes[i + 2];

            const auto it0 = tag_to_index.find(t0);
            const auto it1 = tag_to_index.find(t1);
            const auto it2 = tag_to_index.find(t2);

            if (it0 == tag_to_index.end() ||
                it1 == tag_to_index.end() ||
                it2 == tag_to_index.end())
            {
                throw std::runtime_error(
                    "GmshTriangleMeshGenerator: element references unknown node tag."
                );
            }

            triangles.push_back({
                it0->second,
                it1->second,
                it2->second
            });
        }
    }

    if (triangles.empty())
    {
        throw std::runtime_error(
            "GmshTriangleMeshGenerator: gmsh generated no Triangle3 elements."
        );
    }

    return TriangleMesh::create_unstructured(
        points,
        triangles
    );
}

} // namespace

TriangleMesh GmshTriangleMeshGenerator::create_unstructured_rectangle(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    const GmshTriangleMeshOptions& options
)
{
    check_rectangle_args(
        xmin,
        xmax,
        ymin,
        ymax
    );

    if (!(options.mesh_size > 0.0))
    {
        throw std::invalid_argument(
            "GmshTriangleMeshGenerator::create_unstructured_rectangle: mesh_size must be positive."
        );
    }

    ensure_gmsh_initialized(
        options.verbose
    );

    gmsh::clear();

    gmsh::model::add(
        options.model_name.empty()
            ? "OpenCAX_Gmsh_UnstructuredTriangleMesh"
            : options.model_name
    );

    gmsh::option::setNumber(
        "Mesh.Algorithm",
        static_cast<double>(options.algorithm)
    );

    int p1 = 0;
    int p2 = 0;
    int p3 = 0;
    int p4 = 0;

    int l1 = 0;
    int l2 = 0;
    int l3 = 0;
    int l4 = 0;

    create_rectangle_geometry(
        xmin,
        xmax,
        ymin,
        ymax,
        options.mesh_size,
        p1,
        p2,
        p3,
        p4,
        l1,
        l2,
        l3,
        l4
    );

    gmsh::model::geo::synchronize();

    gmsh::model::mesh::setOrder(1);

    gmsh::model::mesh::generate(2);

    if (options.optimize)
    {
        gmsh::model::mesh::optimize("Netgen");
    }

    TriangleMesh mesh =
        read_triangle_mesh_from_gmsh();

    mesh.set_name("GmshUnstructuredTriangleMesh");
    mesh.set_source("Gmsh::Unstructured");
    mesh.set_source_type(
        TriangleMeshSourceType::GmshUnstructured
    );

    return mesh;
}

TriangleMesh GmshTriangleMeshGenerator::create_structured_rectangle(
    double xmin,
    double xmax,
    double ymin,
    double ymax,
    int nx,
    int ny,
    const GmshTriangleMeshOptions& options
)
{
    check_rectangle_args(
        xmin,
        xmax,
        ymin,
        ymax
    );

    check_structured_args(
        nx,
        ny
    );

    ensure_gmsh_initialized(
        options.verbose
    );

    gmsh::clear();

    gmsh::model::add(
        options.model_name.empty()
            ? "OpenCAX_Gmsh_StructuredTriangleMesh"
            : options.model_name
    );

    int p1 = 0;
    int p2 = 0;
    int p3 = 0;
    int p4 = 0;

    int l1 = 0;
    int l2 = 0;
    int l3 = 0;
    int l4 = 0;

    const double hx =
        (xmax - xmin) / static_cast<double>(nx);

    const double hy =
        (ymax - ymin) / static_cast<double>(ny);

    const double mesh_size =
        hx < hy ? hx : hy;

    const int surface =
        create_rectangle_geometry(
            xmin,
            xmax,
            ymin,
            ymax,
            mesh_size,
            p1,
            p2,
            p3,
            p4,
            l1,
            l2,
            l3,
            l4
        );

    /*
     * TransfiniteCurve 第二个参数是点数，不是单元数。
     */
    gmsh::model::geo::mesh::setTransfiniteCurve(
        l1,
        nx + 1
    );

    gmsh::model::geo::mesh::setTransfiniteCurve(
        l3,
        nx + 1
    );

    gmsh::model::geo::mesh::setTransfiniteCurve(
        l2,
        ny + 1
    );

    gmsh::model::geo::mesh::setTransfiniteCurve(
        l4,
        ny + 1
    );

    /*
     * 设置结构化曲面。
     *
     * 不调用 setRecombine，
     * 因此 Gmsh 会生成 Triangle3，
     * 而不是 Quad4。
     */
    gmsh::model::geo::mesh::setTransfiniteSurface(
        surface,
        "Left",
        {p1, p2, p3, p4}
    );

    gmsh::model::geo::synchronize();

    gmsh::model::mesh::setOrder(1);

    gmsh::model::mesh::generate(2);

    /*
     * 结构网格不建议优化。
     *
     * 如果 optimize=true，Gmsh 可能移动节点，
     * 导致结构网格规则性被破坏。
     */
    if (options.optimize)
    {
        gmsh::model::mesh::optimize("Netgen");
    }

    TriangleMesh mesh =
        read_triangle_mesh_from_gmsh();

    TriangleMeshStructuredInfo info;
    info.nx = nx;
    info.ny = ny;
    info.xmin = xmin;
    info.xmax = xmax;
    info.ymin = ymin;
    info.ymax = ymax;
    info.alternate_diagonal = false;

    mesh.set_structured_info(info);

    mesh.set_name("GmshStructuredTriangleMesh");
    mesh.set_source("Gmsh::Structured");
    mesh.set_source_type(
        TriangleMeshSourceType::GmshStructured
    );

    return mesh;
}

void GmshTriangleMeshGenerator::finalize()
{
    if (gmsh::isInitialized())
    {
        gmsh::finalize();
    }
}

} // namespace OpenCAX
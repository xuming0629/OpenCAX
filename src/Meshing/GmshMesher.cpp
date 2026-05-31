#include <OpenCAX/Meshing/GmshMesher.h>
#include <OpenCAX/IO/OccIO.h>
#include <OpenCAX/Core/Logger.h>

#include <gmsh.h>

#include <cstdio>
#include <string>
#include <vector>
#include <exception>

namespace OpenCAX
{

Mesh GmshMesher::generateSurfaceMesh(
    const TopoDS_Shape& shape,
    const MeshingOptions& options
)
{
    return generate(shape, options, 2);
}

Mesh GmshMesher::generateVolumeMesh(
    const TopoDS_Shape& shape,
    const MeshingOptions& options
)
{
    return generate(shape, options, 3);
}

Mesh GmshMesher::generate(
    const TopoDS_Shape& shape,
    const MeshingOptions& options,
    int dim
)
{
    Mesh mesh;

    const std::string tmp_brep = "opencax_tmp_shape.brep";

    if (!OccIO::writeBREP(shape, tmp_brep)) {
        Logger::error("Failed to write temporary BREP for Gmsh.");
        return mesh;
    }

    gmsh::initialize();

    try {
        gmsh::option::setNumber("General.Terminal", options.verbose ? 1 : 0);

        gmsh::model::add("OpenCAXModel");

        gmsh::vectorpair imported;

        gmsh::model::occ::importShapes(
            tmp_brep,
            imported,
            true
        );

        if (imported.empty()) {
            Logger::error("Gmsh failed to import shape.");
            gmsh::finalize();
            std::remove(tmp_brep.c_str());
            return mesh;
        }

        gmsh::model::occ::synchronize();

        gmsh::option::setNumber("Mesh.MeshSizeMin", options.mesh_size_min);
        gmsh::option::setNumber("Mesh.MeshSizeMax", options.mesh_size_max);
        gmsh::option::setNumber("Mesh.ElementOrder", options.second_order ? 2 : 1);

        gmsh::model::mesh::generate(dim);

        if (options.optimize) {
            gmsh::model::mesh::optimize("Netgen");
        }

        std::vector<std::size_t> node_tags;
        std::vector<double> coords;
        std::vector<double> params;

        gmsh::model::mesh::getNodes(node_tags, coords, params);

        mesh.nodes().reserve(node_tags.size());

        for (std::size_t i = 0; i < node_tags.size(); ++i) {
            MeshNode node;
            node.x = coords[3 * i + 0];
            node.y = coords[3 * i + 1];
            node.z = coords[3 * i + 2];
            mesh.nodes().push_back(node);
        }

        std::vector<int> elem_types;
        std::vector<std::vector<std::size_t>> elem_tags;
        std::vector<std::vector<std::size_t>> elem_node_tags;

        gmsh::model::mesh::getElements(
            elem_types,
            elem_tags,
            elem_node_tags
        );

        for (std::size_t block = 0; block < elem_types.size(); ++block) {
            int elem_type = elem_types[block];

            std::string name;
            int dim_elem = 0;
            int order = 0;
            int num_nodes = 0;
            std::vector<double> local_coords;
            int num_primary_nodes = 0;

            gmsh::model::mesh::getElementProperties(
                elem_type,
                name,
                dim_elem,
                order,
                num_nodes,
                local_coords,
                num_primary_nodes
            );

            const auto& conn = elem_node_tags[block];

            for (std::size_t i = 0; i < conn.size(); i += num_nodes) {
                MeshCell cell;
                cell.type = elem_type;

                for (int j = 0; j < num_nodes; ++j) {
                    cell.node_ids.push_back(
                        static_cast<int>(conn[i + j] - 1)
                    );
                }

                mesh.cells().push_back(std::move(cell));
            }
        }
    }
    catch (const std::exception& e) {
        Logger::error(std::string("Gmsh error: ") + e.what());
    }

    gmsh::finalize();

    std::remove(tmp_brep.c_str());

    return mesh;
}

}
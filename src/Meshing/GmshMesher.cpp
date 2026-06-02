#include <OpenCAX/Meshing/GmshMesher.h>

#include <OpenCAX/Core/LogMacros.h>
#include <OpenCAX/IO/OccIO.h>

#include <gmsh.h>

#include <cstdio>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>

namespace OpenCAX
{

static std::string makeTempBrepName()
{
    return "opencax_gmsh_temp_shape.brep";
}

static void setGmshOptions(const MeshingOptions& options)
{
    gmsh::option::setNumber(
        "General.Terminal",
        options.verbose ? 1 : 0
    );

    gmsh::option::setNumber(
        "Mesh.CharacteristicLengthMin",
        options.mesh_size_min
    );

    gmsh::option::setNumber(
        "Mesh.CharacteristicLengthMax",
        options.mesh_size_max
    );

    gmsh::option::setNumber(
        "Mesh.ElementOrder",
        options.second_order ? 2 : 1
    );

    gmsh::option::setNumber(
        "Mesh.Algorithm",
        options.algorithm_2d
    );

    gmsh::option::setNumber(
        "Mesh.Algorithm3D",
        options.algorithm_3d
    );

    gmsh::option::setNumber(
        "Mesh.Smoothing",
        options.smoothing_steps
    );

    if (options.recombine) {
        gmsh::option::setNumber("Mesh.RecombineAll", 1);
        gmsh::option::setNumber("Mesh.Algorithm", 8);
        gmsh::option::setNumber("Mesh.Algorithm3D", 8);
        gmsh::option::setNumber("Mesh.SubdivisionAlgorithm", 2);
    }
}

static int gmshElementNodeCount(int elem_type)
{
    switch (elem_type) {
    case 1:
        return 2;   // Line2
    case 2:
        return 3;   // Triangle3
    case 3:
        return 4;   // Quad4
    case 4:
        return 4;   // Tetra4
    case 5:
        return 8;   // Hexa8
    case 6:
        return 6;   // Prism6
    case 7:
        return 5;   // Pyramid5
    case 8:
        return 3;   // Line3
    case 9:
        return 6;   // Triangle6
    case 10:
        return 9;   // Quad9
    case 11:
        return 10;  // Tetra10
    case 12:
        return 27;  // Hexa27
    case 16:
        return 8;   // Quad8
    case 17:
        return 20;  // Hexa20
    default:
        return 0;
    }
}

static CellType gmshElementToCellType(int elem_type)
{
    switch (elem_type) {
    case 1:
        return CellType::Line2;
    case 2:
        return CellType::Triangle3;
    case 3:
        return CellType::Quad4;
    case 4:
        return CellType::Tetra4;
    case 5:
        return CellType::Hexa8;
    case 6:
        return CellType::Prism6;
    case 7:
        return CellType::Pyramid5;
    case 8:
        return CellType::Line3;
    case 9:
        return CellType::Triangle6;
    case 10:
        return CellType::Quad9;
    case 11:
        return CellType::Tetra10;
    case 12:
        return CellType::Hexa27;
    case 16:
        return CellType::Quad8;
    case 17:
        return CellType::Hexa20;
    default:
        return CellType::Unknown;
    }
}

static void applyTransfiniteMeshing(const MeshingOptions& options, int dim)
{
    // ===============================
    // curves
    // ===============================
    gmsh::vectorpair curves;
    gmsh::model::getEntities(curves, 1);

    for (const auto& curve : curves) {
        gmsh::model::mesh::setTransfiniteCurve(
            curve.second,
            options.nx
        );
    }

    // ===============================
    // surfaces
    // ===============================
    gmsh::vectorpair surfaces;
    gmsh::model::getEntities(surfaces, 2);

    for (const auto& surface : surfaces) {
        gmsh::model::mesh::setTransfiniteSurface(
            surface.second
        );

        gmsh::model::mesh::setRecombine(
            2,
            surface.second
        );
    }

    // ===============================
    // volumes
    // ===============================
    if (dim == 3) {
        gmsh::vectorpair volumes;
        gmsh::model::getEntities(volumes, 3);

        for (const auto& volume : volumes) {
            gmsh::model::mesh::setTransfiniteVolume(
                volume.second
            );

            gmsh::model::mesh::setRecombine(
                3,
                volume.second
            );
        }
    }
}

static Mesh convertGmshToOpenCAXMesh(int dim)
{
    Mesh mesh;

    if (dim == 1) {
        mesh.info().topology_dim = TopologyDim::Dim1;
        mesh.info().kind = MeshKind::Curve;
    } else if (dim == 2) {
        mesh.info().topology_dim = TopologyDim::Dim2;
        mesh.info().kind = MeshKind::Surface;
    } else {
        mesh.info().topology_dim = TopologyDim::Dim3;
        mesh.info().kind = MeshKind::Volume;
    }

    mesh.info().geometry_dim = GeometryDim::Dim3;
    mesh.info().structure = MeshStructure::Unstructured;

    std::vector<std::size_t> node_tags;
    std::vector<double> node_coords;
    std::vector<double> node_params;

    gmsh::model::mesh::getNodes(
        node_tags,
        node_coords,
        node_params
    );

    std::unordered_map<std::size_t, int> tag_to_index;
    tag_to_index.reserve(node_tags.size());

    for (std::size_t i = 0; i < node_tags.size(); ++i) {
        const double x = node_coords[3 * i + 0];
        const double y = node_coords[3 * i + 1];
        const double z = node_coords[3 * i + 2];

        const int node_id = mesh.add_node(x, y, z);
        tag_to_index[node_tags[i]] = node_id;
    }

    std::vector<int> element_types;
    std::vector<std::vector<std::size_t>> element_tags;
    std::vector<std::vector<std::size_t>> element_node_tags;

    gmsh::model::mesh::getElements(
        element_types,
        element_tags,
        element_node_tags,
        dim
    );

    for (std::size_t i = 0; i < element_types.size(); ++i) {
        const int elem_type = element_types[i];

        const int num_nodes_per_elem =
            gmshElementNodeCount(elem_type);

        const CellType cell_type =
            gmshElementToCellType(elem_type);

        if (num_nodes_per_elem == 0 ||
            cell_type == CellType::Unknown) {

            OpenCAX_LOG_WARN(
                LogModule::Meshing,
                "Unsupported Gmsh element type: {}",
                elem_type
            );

            continue;
        }

        const auto& nodes = element_node_tags[i];

        for (std::size_t j = 0;
             j < nodes.size();
             j += num_nodes_per_elem) {

            std::vector<int> cell;
            cell.reserve(num_nodes_per_elem);

            for (int k = 0; k < num_nodes_per_elem; ++k) {
                const std::size_t gmsh_node_tag =
                    nodes[j + k];

                auto it = tag_to_index.find(gmsh_node_tag);

                if (it == tag_to_index.end()) {
                    OpenCAX_LOG_WARN(
                        LogModule::Meshing,
                        "Gmsh node tag not found: {}",
                        gmsh_node_tag
                    );
                    continue;
                }

                cell.push_back(it->second);
            }

            if (static_cast<int>(cell.size()) ==
                num_nodes_per_elem) {

                mesh.add_cell(cell_type, cell);
            }
        }
    }

    return mesh;
}

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

    if (shape.IsNull()) {
        OpenCAX_LOG_ERROR(
            LogModule::Meshing,
            "Input shape is null"
        );

        return mesh;
    }

    const std::string tmp_brep =
        makeTempBrepName();

    if (!OccIO::writeBREP(shape, tmp_brep)) {
        OpenCAX_LOG_ERROR(
            LogModule::Meshing,
            "Failed to write temporary BREP file: {}",
            tmp_brep
        );

        return mesh;
    }

    bool gmsh_initialized = false;

    try {
        gmsh::initialize();
        gmsh_initialized = true;

        gmsh::model::add(
            "OpenCAX_Gmsh_Mesh"
        );

        setGmshOptions(options);

        std::vector<std::pair<int, int>> imported_entities;

        gmsh::model::occ::importShapes(
            tmp_brep,
            imported_entities
        );

        gmsh::model::occ::synchronize();

        OpenCAX_LOG_INFO(
            LogModule::Meshing,
            "Imported shape into Gmsh, entities = {}",
            imported_entities.size()
        );

        if (options.transfinite) {
            applyTransfiniteMeshing(
                options,
                dim
            );
        }

        gmsh::model::mesh::generate(dim);

        if (options.optimize) {
            gmsh::model::mesh::optimize(
                "Netgen"
            );
        }

        if (options.save_msh) {
            gmsh::write(
                options.msh_file
            );

            OpenCAX_LOG_INFO(
                LogModule::Meshing,
                "Saved Gmsh mesh file: {}",
                options.msh_file
            );
        }

        mesh = convertGmshToOpenCAXMesh(
            dim
        );

        mesh.info().is_second_order =
            options.second_order;

        OpenCAX_LOG_INFO(
            LogModule::Meshing,
            "Generated mesh: nodes = {}, cells = {}",
            mesh.num_nodes(),
            mesh.num_cells()
        );
    }
    catch (const std::exception& e) {
        OpenCAX_LOG_ERROR(
            LogModule::Meshing,
            "Gmsh meshing failed: {}",
            e.what()
        );
    }

    if (gmsh_initialized) {
        try {
            gmsh::finalize();
        } catch (...) {
        }
    }

    if (!options.save_temp_brep) {
        std::remove(
            tmp_brep.c_str()
        );
    }

    return mesh;
}

} // namespace OpenCAX
/**
 * @file MeshPlotter.cpp
 * @brief OpenCAX 网格绘图工具类实现
 */

#include <OpenCAX/Post/Plot/MeshPlotter.h>
#include <OpenCAX/Mesh/CellTopology.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace OpenCAX
{

bool MeshPlotter::show_mesh(
    const Mesh& mesh,
    const MeshTopology& topology,
    const MeshPlotOptions& options
)
{
    return run_plot(mesh, topology, MeshPlotMode::ShowMesh, -1, options);
}

bool MeshPlotter::find_node(
    const Mesh& mesh,
    const MeshTopology& topology,
    int node_id,
    const MeshPlotOptions& options
)
{
    if (!mesh.valid_node_id(node_id))
    {
        return false;
    }

    return run_plot(mesh, topology, MeshPlotMode::FindNode, node_id, options);
}

bool MeshPlotter::find_edge(
    const Mesh& mesh,
    const MeshTopology& topology,
    int edge_id,
    const MeshPlotOptions& options
)
{
    if (edge_id < 0 || static_cast<std::size_t>(edge_id) >= topology.edges().size())
    {
        return false;
    }

    return run_plot(mesh, topology, MeshPlotMode::FindEdge, edge_id, options);
}

bool MeshPlotter::find_face(
    const Mesh& mesh,
    const MeshTopology& topology,
    int face_id,
    const MeshPlotOptions& options
)
{
    if (face_id < 0 || static_cast<std::size_t>(face_id) >= topology.faces().size())
    {
        return false;
    }

    return run_plot(mesh, topology, MeshPlotMode::FindFace, face_id, options);
}

bool MeshPlotter::find_cell(
    const Mesh& mesh,
    const MeshTopology& topology,
    int cell_id,
    const MeshPlotOptions& options
)
{
    if (!mesh.valid_cell_id(cell_id))
    {
        return false;
    }

    return run_plot(mesh, topology, MeshPlotMode::FindCell, cell_id, options);
}

bool MeshPlotter::run_plot(
    const Mesh& mesh,
    const MeshTopology& topology,
    MeshPlotMode mode,
    int target_id,
    const MeshPlotOptions& options
)
{
    std::string json_path = options.json_path;

    if (json_path.empty())
    {
        json_path = make_temp_json_path();
    }

    if (!write_json(mesh, topology, mode, target_id, options, json_path))
    {
        return false;
    }

    std::ostringstream cmd;

    cmd << quote_path(options.python_executable)
        << " "
        << quote_path(options.script_path)
        << " --input "
        << quote_path(json_path);

    const int ret = std::system(cmd.str().c_str());

    return ret == 0;
}

bool MeshPlotter::write_json(
    const Mesh& mesh,
    const MeshTopology& topology,
    MeshPlotMode mode,
    int target_id,
    const MeshPlotOptions& options,
    const std::string& json_path
)
{
    const std::filesystem::path path(json_path);

    if (path.has_parent_path())
    {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream ofs(json_path);

    if (!ofs.is_open())
    {
        return false;
    }

    ofs << "{\n";

    ofs << "  \"mode\": \"" << mode_to_string(mode) << "\",\n";
    ofs << "  \"target_id\": " << target_id << ",\n";

    ofs << "  \"options\": {\n";
    ofs << "    \"show\": " << (options.show ? "true" : "false") << ",\n";
    ofs << "    \"show_node_id\": " << (options.show_node_id ? "true" : "false") << ",\n";
    ofs << "    \"show_edge_id\": " << (options.show_edge_id ? "true" : "false") << ",\n";
    ofs << "    \"show_face_id\": " << (options.show_face_id ? "true" : "false") << ",\n";
    ofs << "    \"show_cell_id\": " << (options.show_cell_id ? "true" : "false") << ",\n";
    ofs << "    \"show_nodes\": " << (options.show_nodes ? "true" : "false") << ",\n";
    ofs << "    \"show_edges\": " << (options.show_edges ? "true" : "false") << ",\n";
    ofs << "    \"show_faces\": " << (options.show_faces ? "true" : "false") << ",\n";
    ofs << "    \"show_wireframe\": " << (options.show_wireframe ? "true" : "false") << ",\n";
    ofs << "    \"show_boundary_only\": " << (options.show_boundary_only ? "true" : "false") << ",\n";
    ofs << "    \"title\": \"" << json_escape(options.title) << "\",\n";
    ofs << "    \"output_path\": \"" << json_escape(options.output_path) << "\"\n";
    ofs << "  },\n";

    ofs << "  \"mesh_info\": {\n";
    ofs << "    \"name\": \"" << json_escape(mesh.info().name) << "\",\n";
    ofs << "    \"source\": \"" << json_escape(mesh.info().source) << "\",\n";
    // ofs << "    \"unit\": \"" << json_escape(mesh.info().unit) << "\"\n";
    ofs << "  },\n";

    ofs << "  \"nodes\": [\n";

    for (std::size_t i = 0; i < mesh.nodes().size(); ++i)
    {
        const auto& node = mesh.nodes()[i];

        ofs << "    {"
            << "\"id\": " << node.id << ", "
            << "\"x\": " << node.x << ", "
            << "\"y\": " << node.y << ", "
            << "\"z\": " << node.z
            << "}";

        if (i + 1 < mesh.nodes().size())
        {
            ofs << ",";
        }

        ofs << "\n";
    }

    ofs << "  ],\n";

    ofs << "  \"cells\": [\n";

    for (std::size_t i = 0; i < mesh.cells().size(); ++i)
    {
        const auto& cell = mesh.cells()[i];

        ofs << "    {"
            << "\"id\": " << cell.id << ", "
            << "\"type\": \"" << CellTopology::to_string(cell.type) << "\", "
            << "\"node_ids\": [";

        for (std::size_t j = 0; j < cell.node_ids.size(); ++j)
        {
            ofs << cell.node_ids[j];

            if (j + 1 < cell.node_ids.size())
            {
                ofs << ", ";
            }
        }

        ofs << "]}";

        if (i + 1 < mesh.cells().size())
        {
            ofs << ",";
        }

        ofs << "\n";
    }

    ofs << "  ],\n";

    ofs << "  \"edges\": [\n";

    for (std::size_t i = 0; i < topology.edges().size(); ++i)
    {
        const auto& edge = topology.edges()[i];

        ofs << "    {"
            << "\"id\": " << i << ", "
            << "\"node_ids\": [" << edge[0] << ", " << edge[1] << "]"
            << "}";

        if (i + 1 < topology.edges().size())
        {
            ofs << ",";
        }

        ofs << "\n";
    }

    ofs << "  ],\n";

    ofs << "  \"faces\": [\n";

    for (std::size_t i = 0; i < topology.faces().size(); ++i)
    {
        const auto& face = topology.faces()[i];

        ofs << "    {"
            << "\"id\": " << i << ", "
            << "\"type\": \"" << CellTopology::to_string(topology.face_types()[i]) << "\", "
            << "\"node_ids\": [";

        for (std::size_t j = 0; j < face.size(); ++j)
        {
            ofs << face[j];

            if (j + 1 < face.size())
            {
                ofs << ", ";
            }
        }

        ofs << "]}";

        if (i + 1 < topology.faces().size())
        {
            ofs << ",";
        }

        ofs << "\n";
    }

    ofs << "  ],\n";

    ofs << "  \"cell2edge\": [\n";

    for (std::size_t i = 0; i < topology.cell2edge().size(); ++i)
    {
        ofs << "    {\"cell_id\": " << i << ", \"edge_ids\": [";

        const auto& edge_ids = topology.cell2edge()[i];

        for (std::size_t j = 0; j < edge_ids.size(); ++j)
        {
            ofs << edge_ids[j];

            if (j + 1 < edge_ids.size())
            {
                ofs << ", ";
            }
        }

        ofs << "]}";

        if (i + 1 < topology.cell2edge().size())
        {
            ofs << ",";
        }

        ofs << "\n";
    }

    ofs << "  ],\n";

    ofs << "  \"cell2face\": [\n";

    for (std::size_t i = 0; i < topology.cell2face().size(); ++i)
    {
        ofs << "    {\"cell_id\": " << i << ", \"face_ids\": [";

        const auto& face_ids = topology.cell2face()[i];

        for (std::size_t j = 0; j < face_ids.size(); ++j)
        {
            ofs << face_ids[j];

            if (j + 1 < face_ids.size())
            {
                ofs << ", ";
            }
        }

        ofs << "]}";

        if (i + 1 < topology.cell2face().size())
        {
            ofs << ",";
        }

        ofs << "\n";
    }

    ofs << "  ],\n";

    ofs << "  \"boundary_edges\": [";

    for (std::size_t i = 0; i < topology.boundary_edges().size(); ++i)
    {
        ofs << topology.boundary_edges()[i];

        if (i + 1 < topology.boundary_edges().size())
        {
            ofs << ", ";
        }
    }

    ofs << "],\n";

    ofs << "  \"boundary_faces\": [";

    for (std::size_t i = 0; i < topology.boundary_faces().size(); ++i)
    {
        ofs << topology.boundary_faces()[i];

        if (i + 1 < topology.boundary_faces().size())
        {
            ofs << ", ";
        }
    }

    ofs << "],\n";

    ofs << "  \"boundary_nodes\": [";

    for (std::size_t i = 0; i < topology.boundary_nodes().size(); ++i)
    {
        ofs << topology.boundary_nodes()[i];

        if (i + 1 < topology.boundary_nodes().size())
        {
            ofs << ", ";
        }
    }

    ofs << "],\n";

    ofs << "  \"boundary_cells\": [";

    for (std::size_t i = 0; i < topology.boundary_cells().size(); ++i)
    {
        ofs << topology.boundary_cells()[i];

        if (i + 1 < topology.boundary_cells().size())
        {
            ofs << ", ";
        }
    }

    ofs << "]\n";

    ofs << "}\n";

    return true;
}

std::string MeshPlotter::mode_to_string(
    MeshPlotMode mode
)
{
    switch (mode)
    {
    case MeshPlotMode::ShowMesh:
        return "show_mesh";

    case MeshPlotMode::FindNode:
        return "find_node";

    case MeshPlotMode::FindEdge:
        return "find_edge";

    case MeshPlotMode::FindFace:
        return "find_face";

    case MeshPlotMode::FindCell:
        return "find_cell";

    default:
        return "show_mesh";
    }
}

std::string MeshPlotter::json_escape(
    const std::string& text
)
{
    std::ostringstream oss;

    for (char c : text)
    {
        switch (c)
        {
        case '"':
            oss << "\\\"";
            break;

        case '\\':
            oss << "\\\\";
            break;

        case '\n':
            oss << "\\n";
            break;

        case '\r':
            oss << "\\r";
            break;

        case '\t':
            oss << "\\t";
            break;

        default:
            oss << c;
            break;
        }
    }

    return oss.str();
}

std::string MeshPlotter::quote_path(
    const std::string& path
)
{
    std::string result = "'";

    for (char c : path)
    {
        if (c == '\'')
        {
            result += "'\\''";
        }
        else
        {
            result += c;
        }
    }

    result += "'";

    return result;
}

std::string MeshPlotter::make_temp_json_path()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto count = now.time_since_epoch().count();

    const auto temp_dir = std::filesystem::temp_directory_path();

    std::filesystem::path path = temp_dir / (
        "opencax_mesh_plot_" + std::to_string(count) + ".json"
    );

    return path.string();
}

} // namespace OpenCAX
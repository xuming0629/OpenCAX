#include <OpenCAX/Mesh2/Mesh.h>
#include <OpenCAX/Mesh2/CellTopology.h>

#include <algorithm>
#include <limits>

namespace OpenCAX
{

const char* to_string(CellType type)
{
    return CellTopology::to_string(type);
}

const char* to_string(MeshDimension d)
{
    switch (d)
    {
    case MeshDimension::Dim0: return "Dim0";
    case MeshDimension::Dim1: return "Dim1";
    case MeshDimension::Dim2: return "Dim2";
    case MeshDimension::Dim3: return "Dim3";
    default: return "Unknown";
    }
}

const char* to_string(MeshSourceType s)
{
    switch (s)
    {
    case MeshSourceType::Structured: return "Structured";
    case MeshSourceType::Unstructured: return "Unstructured";
    case MeshSourceType::Gmsh: return "Gmsh";
    case MeshSourceType::TetGen: return "TetGen";
    case MeshSourceType::Imported: return "Imported";
    case MeshSourceType::Refined: return "Refined";
    case MeshSourceType::HighOrderConverted: return "HighOrderConverted";
    default: return "Unknown";
    }
}

void Mesh::clear()
{
    info_ = MeshInfo{};
    nodes_.clear();
    cells_.clear();
}

MeshInfo& Mesh::info() { return info_; }
const MeshInfo& Mesh::info() const { return info_; }

void Mesh::set_name(const std::string& name) { info_.name = name; }
void Mesh::set_source(const std::string& source) { info_.source = source; }
void Mesh::set_dimension(MeshDimension dim) { info_.dimension = dim; }
void Mesh::set_source_type(MeshSourceType type) { info_.source_type = type; }

std::vector<MeshNode>& Mesh::nodes() { return nodes_; }
const std::vector<MeshNode>& Mesh::nodes() const { return nodes_; }
std::vector<MeshCell>& Mesh::cells() { return cells_; }
const std::vector<MeshCell>& Mesh::cells() const { return cells_; }

int Mesh::add_node(double x, double y, double z)
{
    const int id = static_cast<int>(nodes_.size());
    MeshNode node;
    node.id = id;
    node.x = x;
    node.y = y;
    node.z = z;
    nodes_.push_back(node);
    return id;
}

int Mesh::add_node(double x, double y, double z, int physical_id, int boundary_id, int geometry_id)
{
    const int id = static_cast<int>(nodes_.size());
    MeshNode node;
    node.id = id;
    node.x = x;
    node.y = y;
    node.z = z;
    node.physical_id = physical_id;
    node.boundary_id = boundary_id;
    node.geometry_id = geometry_id;
    nodes_.push_back(node);
    return id;
}

int Mesh::add_cell(CellType type, const std::vector<int>& node_ids)
{
    const int id = static_cast<int>(cells_.size());
    MeshCell cell;
    cell.id = id;
    cell.type = type;
    cell.node_ids = node_ids;
    cell.order = CellTopology::order(type);
    cells_.push_back(cell);
    return id;
}

int Mesh::add_cell(CellType type, const std::vector<int>& node_ids,
                   int physical_id, int material_id, int region_id, int geometry_id)
{
    const int id = static_cast<int>(cells_.size());
    MeshCell cell;
    cell.id = id;
    cell.type = type;
    cell.node_ids = node_ids;
    cell.order = CellTopology::order(type);
    cell.physical_id = physical_id;
    cell.material_id = material_id;
    cell.region_id = region_id;
    cell.geometry_id = geometry_id;
    cells_.push_back(cell);
    return id;
}

bool Mesh::valid_node_id(int node_id) const
{
    return node_id >= 0 && static_cast<std::size_t>(node_id) < nodes_.size();
}

bool Mesh::valid_cell_id(int cell_id) const
{
    return cell_id >= 0 && static_cast<std::size_t>(cell_id) < cells_.size();
}

std::size_t Mesh::num_nodes() const { return nodes_.size(); }
std::size_t Mesh::num_cells() const { return cells_.size(); }
bool Mesh::empty() const { return nodes_.empty() && cells_.empty(); }

bool Mesh::validate(std::string* error_message) const
{
    for (std::size_t i = 0; i < nodes_.size(); ++i)
    {
        if (nodes_[i].id != static_cast<int>(i))
        {
            if (error_message) *error_message = "Mesh::validate: non-contiguous node ids.";
            return false;
        }
    }

    for (std::size_t i = 0; i < cells_.size(); ++i)
    {
        const auto& cell = cells_[i];
        if (cell.id != static_cast<int>(i))
        {
            if (error_message) *error_message = "Mesh::validate: non-contiguous cell ids.";
            return false;
        }
        if (cell.type == CellType::Unknown)
        {
            if (error_message) *error_message = "Mesh::validate: unknown cell type.";
            return false;
        }
        const int expected = CellTopology::num_nodes(cell.type);
        if (expected > 0 && static_cast<int>(cell.node_ids.size()) != expected)
        {
            if (error_message) *error_message = "Mesh::validate: cell node count mismatch.";
            return false;
        }
        for (int nid : cell.node_ids)
        {
            if (!valid_node_id(nid))
            {
                if (error_message) *error_message = "Mesh::validate: invalid node id in cell.";
                return false;
            }
        }
    }

    if (error_message) error_message->clear();
    return true;
}

std::array<double, 6> Mesh::bounds() const
{
    if (nodes_.empty()) return {0, 0, 0, 0, 0, 0};

    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double zmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();
    double zmax = std::numeric_limits<double>::lowest();

    for (const auto& n : nodes_)
    {
        xmin = std::min(xmin, n.x); xmax = std::max(xmax, n.x);
        ymin = std::min(ymin, n.y); ymax = std::max(ymax, n.y);
        zmin = std::min(zmin, n.z); zmax = std::max(zmax, n.z);
    }

    return {xmin, xmax, ymin, ymax, zmin, zmax};
}

} // namespace OpenCAX

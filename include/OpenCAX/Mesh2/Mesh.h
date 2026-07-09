#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/MeshTypes.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

class OpenCAX_API Mesh
{
public:
    virtual ~Mesh() = default;

    void clear();

    MeshInfo& info();
    const MeshInfo& info() const;

    void set_name(const std::string& name);
    void set_source(const std::string& source);
    void set_dimension(MeshDimension dim);
    void set_source_type(MeshSourceType type);

    std::vector<MeshNode>& nodes();
    const std::vector<MeshNode>& nodes() const;

    std::vector<MeshCell>& cells();
    const std::vector<MeshCell>& cells() const;

    int add_node(double x, double y, double z = 0.0);
    int add_node(double x, double y, double z, int physical_id, int boundary_id = -1, int geometry_id = -1);

    int add_cell(CellType type, const std::vector<int>& node_ids);
    int add_cell(CellType type, const std::vector<int>& node_ids,
                 int physical_id, int material_id = -1, int region_id = -1, int geometry_id = -1);

    bool valid_node_id(int node_id) const;
    bool valid_cell_id(int cell_id) const;

    std::size_t num_nodes() const;
    std::size_t num_cells() const;
    bool empty() const;

    virtual bool validate(std::string* error_message = nullptr) const;

    std::array<double, 6> bounds() const;

protected:
    MeshInfo info_;
    std::vector<MeshNode> nodes_;
    std::vector<MeshCell> cells_;
};

} // namespace OpenCAX

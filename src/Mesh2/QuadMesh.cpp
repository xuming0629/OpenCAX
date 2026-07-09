#include <OpenCAX/Mesh2/QuadMesh.h>

#include <cmath>
#include <stdexcept>

namespace OpenCAX
{

QuadMesh::QuadMesh()
{
    info_.name = "QuadMesh";
    info_.source = "OpenCAX";
    info_.dimension = MeshDimension::Dim2;
}

QuadMesh QuadMesh::create_structured_rectangle(double xmin, double xmax, double ymin, double ymax, int nx, int ny)
{
    if (nx <= 0 || ny <= 0) throw std::invalid_argument("QuadMesh::create_structured_rectangle: nx/ny must be positive.");
    if (!(xmax > xmin) || !(ymax > ymin)) throw std::invalid_argument("QuadMesh::create_structured_rectangle: invalid bounds.");

    QuadMesh mesh;
    mesh.info_.name = "StructuredQuadMesh";
    mesh.info_.source = "OpenCAX::Structured";
    mesh.info_.source_type = MeshSourceType::Structured;
    mesh.info_.structured = true;
    mesh.structured_info_ = {nx, ny, xmin, xmax, ymin, ymax, false};

    const double dx = (xmax - xmin) / nx;
    const double dy = (ymax - ymin) / ny;
    auto nid = [nx](int i, int j) { return j * (nx + 1) + i; };

    for (int j = 0; j <= ny; ++j)
        for (int i = 0; i <= nx; ++i)
            mesh.add_node(xmin + i * dx, ymin + j * dy, 0.0);

    for (int j = 0; j < ny; ++j)
        for (int i = 0; i < nx; ++i)
            mesh.add_cell(CellType::Quad4, {nid(i,j), nid(i+1,j), nid(i+1,j+1), nid(i,j+1)});

    return mesh;
}

QuadMesh QuadMesh::create_unstructured(const std::vector<std::array<double,3>>& points, const std::vector<std::array<int,4>>& quads)
{
    QuadMesh mesh;
    mesh.info_.name = "UnstructuredQuadMesh";
    mesh.info_.source = "OpenCAX::Unstructured";
    mesh.info_.source_type = MeshSourceType::Unstructured;
    for (const auto& p : points) mesh.add_node(p[0], p[1], p[2]);
    for (const auto& q : quads)
    {
        for (int id : q) if (!mesh.valid_node_id(id)) throw std::out_of_range("QuadMesh::create_unstructured: invalid node id.");
        mesh.add_cell(CellType::Quad4, {q[0],q[1],q[2],q[3]});
    }
    return mesh;
}

void QuadMesh::set_structured_info(const StructuredGridInfo2D& info) { structured_info_ = info; }
const StructuredGridInfo2D& QuadMesh::structured_info() const { return structured_info_; }

double QuadMesh::triangle_area(const MeshNode& a, const MeshNode& b, const MeshNode& c)
{
    const double ux = b.x-a.x, uy=b.y-a.y, uz=b.z-a.z;
    const double vx = c.x-a.x, vy=c.y-a.y, vz=c.z-a.z;
    const double cx=uy*vz-uz*vy, cy=uz*vx-ux*vz, cz=ux*vy-uy*vx;
    return 0.5*std::sqrt(cx*cx+cy*cy+cz*cz);
}

double QuadMesh::quad_area(const MeshNode& a, const MeshNode& b, const MeshNode& c, const MeshNode& d)
{
    return triangle_area(a,b,c) + triangle_area(a,c,d);
}

double QuadMesh::area(int cell_id) const
{
    if (!valid_cell_id(cell_id)) return 0.0;
    const auto& c = cells_[cell_id];
    if ((c.type != CellType::Quad4 && c.type != CellType::Quad8 && c.type != CellType::Quad9) || c.node_ids.size() < 4) return 0.0;
    return quad_area(nodes_[c.node_ids[0]], nodes_[c.node_ids[1]], nodes_[c.node_ids[2]], nodes_[c.node_ids[3]]);
}

double QuadMesh::total_area() const
{
    double s=0.0;
    for (std::size_t i=0;i<cells_.size();++i) s += area(static_cast<int>(i));
    return s;
}

std::array<double,3> QuadMesh::centroid(int cell_id) const
{
    if (!valid_cell_id(cell_id)) return {0,0,0};
    const auto& c = cells_[cell_id];
    double x=0,y=0,z=0;
    const int count = std::min<int>(4, c.node_ids.size());
    for (int i=0;i<count;++i) { const auto& p = nodes_[c.node_ids[i]]; x+=p.x; y+=p.y; z+=p.z; }
    return {x/count, y/count, z/count};
}

bool QuadMesh::validate(std::string* error_message, double area_eps) const
{
    if (!Mesh::validate(error_message)) return false;
    for (std::size_t i=0;i<cells_.size();++i)
    {
        const auto& c = cells_[i];
        if (c.type != CellType::Quad4 && c.type != CellType::Quad8 && c.type != CellType::Quad9)
        { if(error_message) *error_message="QuadMesh::validate: unsupported quad cell type."; return false; }
        if (area(static_cast<int>(i)) <= area_eps)
        { if(error_message) *error_message="QuadMesh::validate: non-positive quad area."; return false; }
    }
    if (error_message) error_message->clear();
    return true;
}

bool QuadMesh::validate(std::string* error_message) const { return validate(error_message, 1.0e-14); }

} // namespace OpenCAX

#include <OpenCAX/Mesh/TetraMesh.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace OpenCAX
{

TetraMesh::TetraMesh()
{
    info_.name = "TetraMesh";
    info_.source = "OpenCAX";
    info_.dimension = MeshDimension::Dim3;
}

TetraMesh TetraMesh::create_structured_box(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int nx, int ny, int nz)
{
    if (nx <= 0 || ny <= 0 || nz <= 0) throw std::invalid_argument("TetraMesh::create_structured_box: nx/ny/nz must be positive.");
    if (!(xmax > xmin) || !(ymax > ymin) || !(zmax > zmin)) throw std::invalid_argument("TetraMesh::create_structured_box: invalid bounds.");

    TetraMesh mesh;
    mesh.info_.name = "StructuredTetraMesh";
    mesh.info_.source = "OpenCAX::Structured";
    mesh.info_.source_type = MeshSourceType::Structured;
    mesh.info_.structured = true;
    mesh.structured_info_ = {nx, ny, nz, xmin, xmax, ymin, ymax, zmin, zmax};

    const double dx=(xmax-xmin)/nx, dy=(ymax-ymin)/ny, dz=(zmax-zmin)/nz;
    auto nid=[nx,ny](int i,int j,int k){ return k*(ny+1)*(nx+1)+j*(nx+1)+i; };

    for(int k=0;k<=nz;++k) for(int j=0;j<=ny;++j) for(int i=0;i<=nx;++i)
        mesh.add_node(xmin+i*dx, ymin+j*dy, zmin+k*dz);

    for(int k=0;k<nz;++k) for(int j=0;j<ny;++j) for(int i=0;i<nx;++i)
    {
        int n0=nid(i,j,k), n1=nid(i+1,j,k), n2=nid(i+1,j+1,k), n3=nid(i,j+1,k);
        int n4=nid(i,j,k+1), n5=nid(i+1,j,k+1), n6=nid(i+1,j+1,k+1), n7=nid(i,j+1,k+1);
        mesh.add_cell(CellType::Tetra4,{n0,n1,n2,n6});
        mesh.add_cell(CellType::Tetra4,{n0,n2,n3,n6});
        mesh.add_cell(CellType::Tetra4,{n0,n3,n7,n6});
        mesh.add_cell(CellType::Tetra4,{n0,n7,n4,n6});
        mesh.add_cell(CellType::Tetra4,{n0,n4,n5,n6});
        mesh.add_cell(CellType::Tetra4,{n0,n5,n1,n6});
    }
    return mesh;
}

TetraMesh TetraMesh::create_unstructured(const std::vector<std::array<double,3>>& points, const std::vector<std::array<int,4>>& tets)
{
    TetraMesh mesh;
    mesh.info_.name = "UnstructuredTetraMesh";
    mesh.info_.source = "OpenCAX::Unstructured";
    mesh.info_.source_type = MeshSourceType::Unstructured;
    for (const auto& p: points) mesh.add_node(p[0],p[1],p[2]);
    for (const auto& t: tets)
    {
        for (int id: t) if(!mesh.valid_node_id(id)) throw std::out_of_range("TetraMesh::create_unstructured: invalid node id.");
        mesh.add_cell(CellType::Tetra4,{t[0],t[1],t[2],t[3]});
    }
    return mesh;
}

void TetraMesh::set_structured_info(const StructuredGridInfo3D& info){ structured_info_=info; }
const StructuredGridInfo3D& TetraMesh::structured_info() const { return structured_info_; }

double TetraMesh::signed_tetra_volume6(const MeshNode& a,const MeshNode& b,const MeshNode& c,const MeshNode& d)
{
    const double ux=b.x-a.x, uy=b.y-a.y, uz=b.z-a.z;
    const double vx=c.x-a.x, vy=c.y-a.y, vz=c.z-a.z;
    const double wx=d.x-a.x, wy=d.y-a.y, wz=d.z-a.z;
    return ux*(vy*wz-vz*wy)-uy*(vx*wz-vz*wx)+uz*(vx*wy-vy*wx);
}

double TetraMesh::tetra_volume(const MeshNode& a,const MeshNode& b,const MeshNode& c,const MeshNode& d)
{ return std::abs(signed_tetra_volume6(a,b,c,d))/6.0; }

double TetraMesh::volume(int cell_id) const
{
    if(!valid_cell_id(cell_id)) return 0.0;
    const auto& c=cells_[cell_id];
    if((c.type!=CellType::Tetra4 && c.type!=CellType::Tetra10) || c.node_ids.size()<4) return 0.0;
    return tetra_volume(nodes_[c.node_ids[0]],nodes_[c.node_ids[1]],nodes_[c.node_ids[2]],nodes_[c.node_ids[3]]);
}

double TetraMesh::total_volume() const { double s=0; for(std::size_t i=0;i<cells_.size();++i) s+=volume(static_cast<int>(i)); return s; }

std::array<double,3> TetraMesh::centroid(int cell_id) const
{
    if(!valid_cell_id(cell_id)) return {0,0,0};
    const auto& c=cells_[cell_id]; if(c.node_ids.size()<4) return {0,0,0};
    const auto& a=nodes_[c.node_ids[0]]; const auto& b=nodes_[c.node_ids[1]]; const auto& d=nodes_[c.node_ids[2]]; const auto& e=nodes_[c.node_ids[3]];
    return {(a.x+b.x+d.x+e.x)/4.0,(a.y+b.y+d.y+e.y)/4.0,(a.z+b.z+d.z+e.z)/4.0};
}

bool TetraMesh::validate(std::string* error_message,double volume_eps) const
{
    if(!Mesh::validate(error_message)) return false;
    for(std::size_t i=0;i<cells_.size();++i)
    {
        const auto& c=cells_[i];
        if(c.type!=CellType::Tetra4 && c.type!=CellType::Tetra10) { if(error_message)*error_message="TetraMesh::validate: unsupported tetra cell type."; return false; }
        if(volume(static_cast<int>(i))<=volume_eps){ if(error_message)*error_message="TetraMesh::validate: non-positive tetra volume."; return false; }
    }
    if(error_message) error_message->clear(); return true;
}

bool TetraMesh::validate(std::string* error_message) const { return validate(error_message,1.0e-14); }

} // namespace OpenCAX

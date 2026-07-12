#include <OpenCAX/Mesh/HexMesh.h>
#include <OpenCAX/Mesh/TetraMesh.h>

#include <cmath>
#include <stdexcept>

namespace OpenCAX
{

HexMesh::HexMesh()
{
    info_.name="HexMesh";
    info_.source="OpenCAX";
    info_.dimension=MeshDimension::Dim3;
}

HexMesh HexMesh::create_structured_box(double xmin,double xmax,double ymin,double ymax,double zmin,double zmax,int nx,int ny,int nz)
{
    if(nx<=0||ny<=0||nz<=0) throw std::invalid_argument("HexMesh::create_structured_box: nx/ny/nz must be positive.");
    if(!(xmax>xmin)||!(ymax>ymin)||!(zmax>zmin)) throw std::invalid_argument("HexMesh::create_structured_box: invalid bounds.");
    HexMesh mesh;
    mesh.info_.name="StructuredHexMesh";
    mesh.info_.source="OpenCAX::Structured";
    mesh.info_.source_type=MeshSourceType::Structured;
    mesh.info_.structured=true;
    mesh.structured_info_={nx,ny,nz,xmin,xmax,ymin,ymax,zmin,zmax};
    const double dx=(xmax-xmin)/nx, dy=(ymax-ymin)/ny, dz=(zmax-zmin)/nz;
    auto nid=[nx,ny](int i,int j,int k){return k*(ny+1)*(nx+1)+j*(nx+1)+i;};
    for(int k=0;k<=nz;++k) for(int j=0;j<=ny;++j) for(int i=0;i<=nx;++i)
        mesh.add_node(xmin+i*dx,ymin+j*dy,zmin+k*dz);
    for(int k=0;k<nz;++k) for(int j=0;j<ny;++j) for(int i=0;i<nx;++i)
        mesh.add_cell(CellType::Hexa8,{nid(i,j,k),nid(i+1,j,k),nid(i+1,j+1,k),nid(i,j+1,k),nid(i,j,k+1),nid(i+1,j,k+1),nid(i+1,j+1,k+1),nid(i,j+1,k+1)});
    return mesh;
}

HexMesh HexMesh::create_unstructured(const std::vector<std::array<double,3>>& points,const std::vector<std::array<int,8>>& hexes)
{
    HexMesh mesh; mesh.info_.name="UnstructuredHexMesh"; mesh.info_.source="OpenCAX::Unstructured"; mesh.info_.source_type=MeshSourceType::Unstructured;
    for(const auto& p:points) mesh.add_node(p[0],p[1],p[2]);
    for(const auto& h:hexes){ for(int id:h) if(!mesh.valid_node_id(id)) throw std::out_of_range("HexMesh::create_unstructured: invalid node id."); mesh.add_cell(CellType::Hexa8,{h[0],h[1],h[2],h[3],h[4],h[5],h[6],h[7]}); }
    return mesh;
}

void HexMesh::set_structured_info(const StructuredGridInfo3D& info){structured_info_=info;}
const StructuredGridInfo3D& HexMesh::structured_info() const{return structured_info_;}

double HexMesh::tetra_volume(const MeshNode& a,const MeshNode& b,const MeshNode& c,const MeshNode& d){return TetraMesh::tetra_volume(a,b,c,d);}

double HexMesh::hexa_volume(const MeshNode& a,const MeshNode& b,const MeshNode& c,const MeshNode& d,const MeshNode& e,const MeshNode& f,const MeshNode& g,const MeshNode& h)
{
    return tetra_volume(a,b,c,g)+tetra_volume(a,c,d,g)+tetra_volume(a,d,h,g)+tetra_volume(a,h,e,g)+tetra_volume(a,e,f,g)+tetra_volume(a,f,b,g);
}

double HexMesh::volume(int cell_id) const
{
    if(!valid_cell_id(cell_id)) return 0.0;
    const auto& c=cells_[cell_id];
    if((c.type!=CellType::Hexa8 && c.type!=CellType::Hexa20 && c.type!=CellType::Hexa27)||c.node_ids.size()<8) return 0.0;
    return hexa_volume(nodes_[c.node_ids[0]],nodes_[c.node_ids[1]],nodes_[c.node_ids[2]],nodes_[c.node_ids[3]],nodes_[c.node_ids[4]],nodes_[c.node_ids[5]],nodes_[c.node_ids[6]],nodes_[c.node_ids[7]]);
}

double HexMesh::total_volume() const{double s=0; for(std::size_t i=0;i<cells_.size();++i)s+=volume(static_cast<int>(i)); return s;}

std::array<double,3> HexMesh::centroid(int cell_id) const
{
    if(!valid_cell_id(cell_id)) return {0,0,0}; const auto& c=cells_[cell_id]; if(c.node_ids.size()<8)return{0,0,0};
    double x=0,y=0,z=0; for(int i=0;i<8;++i){const auto& p=nodes_[c.node_ids[i]]; x+=p.x; y+=p.y; z+=p.z;} return {x/8.0,y/8.0,z/8.0};
}

bool HexMesh::validate(std::string* error_message,double volume_eps) const
{
    if(!Mesh::validate(error_message)) return false;
    for(std::size_t i=0;i<cells_.size();++i){const auto& c=cells_[i]; if(c.type!=CellType::Hexa8 && c.type!=CellType::Hexa20 && c.type!=CellType::Hexa27){if(error_message)*error_message="HexMesh::validate: unsupported hex cell type."; return false;} if(volume(static_cast<int>(i))<=volume_eps){if(error_message)*error_message="HexMesh::validate: non-positive hex volume."; return false;}}
    if(error_message)error_message->clear(); return true;
}

bool HexMesh::validate(std::string* error_message) const{return validate(error_message,1.0e-14);}

} // namespace OpenCAX

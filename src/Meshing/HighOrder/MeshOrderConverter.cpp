#include <OpenCAX/Meshing/HighOrder/MeshOrderConverter.h>

#include <array>
#include <map>
#include <stdexcept>

namespace OpenCAX
{
namespace
{
static std::array<int,2> key(int a,int b){return a<b?std::array<int,2>{a,b}:std::array<int,2>{b,a};}

static int mid_node(Mesh& out, const Mesh& in, std::map<std::array<int,2>, int>& mids, int a, int b)
{
    auto k=key(a,b);
    auto it=mids.find(k);
    if(it!=mids.end()) return it->second;
    const auto& pa=in.nodes()[a]; const auto& pb=in.nodes()[b];
    const int id=out.add_node(0.5*(pa.x+pb.x),0.5*(pa.y+pb.y),0.5*(pa.z+pb.z));
    mids[k]=id; return id;
}

static int center_node(Mesh& out, const Mesh& in, const std::vector<int>& ids)
{
    double x=0,y=0,z=0;
    for(int id:ids){const auto& p=in.nodes()[id]; x+=p.x; y+=p.y; z+=p.z;}
    const double s=1.0/static_cast<double>(ids.size());
    return out.add_node(x*s,y*s,z*s);
}
}

TriangleMesh MeshOrderConverter::triangle3_to_triangle6(const TriangleMesh& mesh)
{
    TriangleMesh out;
    out.info() = mesh.info();
    out.set_name(mesh.info().name + "_Triangle6");
    out.set_source_type(MeshSourceType::HighOrderConverted);
    out.info().high_order = true;

    for(const auto& n:mesh.nodes()) out.add_node(n.x,n.y,n.z,n.physical_id,n.boundary_id,n.geometry_id);
    std::map<std::array<int,2>, int> mids;
    for(const auto& c:mesh.cells())
    {
        if(c.type!=CellType::Triangle3 || c.node_ids.size()!=3) throw std::runtime_error("MeshOrderConverter::triangle3_to_triangle6: input must be Triangle3.");
        int v0=c.node_ids[0], v1=c.node_ids[1], v2=c.node_ids[2];
        int m01=mid_node(out,mesh,mids,v0,v1);
        int m12=mid_node(out,mesh,mids,v1,v2);
        int m20=mid_node(out,mesh,mids,v2,v0);
        out.add_cell(CellType::Triangle6,{v0,v1,v2,m01,m12,m20},c.physical_id,c.material_id,c.region_id,c.geometry_id);
    }
    return out;
}

TriangleMesh MeshOrderConverter::to_quadratic(const TriangleMesh& mesh){return triangle3_to_triangle6(mesh);}

QuadMesh MeshOrderConverter::quad4_to_quad8(const QuadMesh& mesh)
{
    QuadMesh out; out.info()=mesh.info(); out.set_name(mesh.info().name+"_Quad8"); out.set_source_type(MeshSourceType::HighOrderConverted); out.info().high_order=true;
    for(const auto& n:mesh.nodes()) out.add_node(n.x,n.y,n.z,n.physical_id,n.boundary_id,n.geometry_id);
    std::map<std::array<int,2>,int> mids;
    for(const auto& c:mesh.cells())
    {
        if(c.type!=CellType::Quad4 || c.node_ids.size()!=4) throw std::runtime_error("quad4_to_quad8: input must be Quad4.");
        int v0=c.node_ids[0],v1=c.node_ids[1],v2=c.node_ids[2],v3=c.node_ids[3];
        out.add_cell(CellType::Quad8,{v0,v1,v2,v3,mid_node(out,mesh,mids,v0,v1),mid_node(out,mesh,mids,v1,v2),mid_node(out,mesh,mids,v2,v3),mid_node(out,mesh,mids,v3,v0)},c.physical_id,c.material_id,c.region_id,c.geometry_id);
    }
    return out;
}

QuadMesh MeshOrderConverter::quad4_to_quad9(const QuadMesh& mesh)
{
    QuadMesh out=quad4_to_quad8(mesh);
    for(auto& c:out.cells()) c.type=CellType::Quad8; // ensure prior state
    // Need rebuild to add per-cell center after shared mids.
    QuadMesh result; result.info()=mesh.info(); result.set_name(mesh.info().name+"_Quad9"); result.set_source_type(MeshSourceType::HighOrderConverted); result.info().high_order=true;
    for(const auto& n:mesh.nodes()) result.add_node(n.x,n.y,n.z,n.physical_id,n.boundary_id,n.geometry_id);
    std::map<std::array<int,2>,int> mids;
    for(const auto& c:mesh.cells())
    {
        int v0=c.node_ids[0],v1=c.node_ids[1],v2=c.node_ids[2],v3=c.node_ids[3];
        int m01=mid_node(result,mesh,mids,v0,v1), m12=mid_node(result,mesh,mids,v1,v2), m23=mid_node(result,mesh,mids,v2,v3), m30=mid_node(result,mesh,mids,v3,v0);
        int cen=center_node(result,mesh,{v0,v1,v2,v3});
        result.add_cell(CellType::Quad9,{v0,v1,v2,v3,m01,m12,m23,m30,cen},c.physical_id,c.material_id,c.region_id,c.geometry_id);
    }
    return result;
}

TetraMesh MeshOrderConverter::tetra4_to_tetra10(const TetraMesh& mesh)
{
    TetraMesh out; out.info()=mesh.info(); out.set_name(mesh.info().name+"_Tetra10"); out.set_source_type(MeshSourceType::HighOrderConverted); out.info().high_order=true;
    for(const auto& n:mesh.nodes()) out.add_node(n.x,n.y,n.z,n.physical_id,n.boundary_id,n.geometry_id);
    std::map<std::array<int,2>,int> mids;
    for(const auto& c:mesh.cells())
    {
        if(c.type!=CellType::Tetra4||c.node_ids.size()!=4) throw std::runtime_error("tetra4_to_tetra10: input must be Tetra4.");
        int v0=c.node_ids[0],v1=c.node_ids[1],v2=c.node_ids[2],v3=c.node_ids[3];
        out.add_cell(CellType::Tetra10,{v0,v1,v2,v3,mid_node(out,mesh,mids,v0,v1),mid_node(out,mesh,mids,v1,v2),mid_node(out,mesh,mids,v2,v0),mid_node(out,mesh,mids,v0,v3),mid_node(out,mesh,mids,v1,v3),mid_node(out,mesh,mids,v2,v3)},c.physical_id,c.material_id,c.region_id,c.geometry_id);
    }
    return out;
}

HexMesh MeshOrderConverter::hexa8_to_hexa20(const HexMesh& mesh)
{
    HexMesh out; out.info()=mesh.info(); out.set_name(mesh.info().name+"_Hexa20"); out.set_source_type(MeshSourceType::HighOrderConverted); out.info().high_order=true;
    for(const auto& n:mesh.nodes()) out.add_node(n.x,n.y,n.z,n.physical_id,n.boundary_id,n.geometry_id);
    std::map<std::array<int,2>,int> mids;
    for(const auto& c:mesh.cells())
    {
        if(c.type!=CellType::Hexa8||c.node_ids.size()!=8) throw std::runtime_error("hexa8_to_hexa20: input must be Hexa8.");
        int v0=c.node_ids[0],v1=c.node_ids[1],v2=c.node_ids[2],v3=c.node_ids[3],v4=c.node_ids[4],v5=c.node_ids[5],v6=c.node_ids[6],v7=c.node_ids[7];
        out.add_cell(CellType::Hexa20,{v0,v1,v2,v3,v4,v5,v6,v7,
            mid_node(out,mesh,mids,v0,v1),mid_node(out,mesh,mids,v1,v2),mid_node(out,mesh,mids,v2,v3),mid_node(out,mesh,mids,v3,v0),
            mid_node(out,mesh,mids,v4,v5),mid_node(out,mesh,mids,v5,v6),mid_node(out,mesh,mids,v6,v7),mid_node(out,mesh,mids,v7,v4),
            mid_node(out,mesh,mids,v0,v4),mid_node(out,mesh,mids,v1,v5),mid_node(out,mesh,mids,v2,v6),mid_node(out,mesh,mids,v3,v7)},c.physical_id,c.material_id,c.region_id,c.geometry_id);
    }
    return out;
}

HexMesh MeshOrderConverter::hexa8_to_hexa27(const HexMesh& mesh)
{
    HexMesh out = hexa8_to_hexa20(mesh);
    out.set_name(mesh.info().name+"_Hexa27");
    // Full Hexa27 face/body node ordering is framework-dependent. Keep edge nodes and append six face centers + body center per cell.
    // This implementation is intentionally explicit and uses OpenCAX internal ordering: 8 vertices + 12 edges + 6 faces + 1 body.
    HexMesh result; result.info()=mesh.info(); result.set_name(mesh.info().name+"_Hexa27"); result.set_source_type(MeshSourceType::HighOrderConverted); result.info().high_order=true;
    for(const auto& n:mesh.nodes()) result.add_node(n.x,n.y,n.z,n.physical_id,n.boundary_id,n.geometry_id);
    std::map<std::array<int,2>,int> mids;
    for(const auto& c:mesh.cells())
    {
        int v0=c.node_ids[0],v1=c.node_ids[1],v2=c.node_ids[2],v3=c.node_ids[3],v4=c.node_ids[4],v5=c.node_ids[5],v6=c.node_ids[6],v7=c.node_ids[7];
        std::vector<int> ids={v0,v1,v2,v3,v4,v5,v6,v7,
            mid_node(result,mesh,mids,v0,v1),mid_node(result,mesh,mids,v1,v2),mid_node(result,mesh,mids,v2,v3),mid_node(result,mesh,mids,v3,v0),
            mid_node(result,mesh,mids,v4,v5),mid_node(result,mesh,mids,v5,v6),mid_node(result,mesh,mids,v6,v7),mid_node(result,mesh,mids,v7,v4),
            mid_node(result,mesh,mids,v0,v4),mid_node(result,mesh,mids,v1,v5),mid_node(result,mesh,mids,v2,v6),mid_node(result,mesh,mids,v3,v7),
            center_node(result,mesh,{v0,v1,v2,v3}),center_node(result,mesh,{v4,v5,v6,v7}),center_node(result,mesh,{v0,v1,v5,v4}),center_node(result,mesh,{v1,v2,v6,v5}),center_node(result,mesh,{v2,v3,v7,v6}),center_node(result,mesh,{v3,v0,v4,v7}),
            center_node(result,mesh,{v0,v1,v2,v3,v4,v5,v6,v7})};
        result.add_cell(CellType::Hexa27,ids,c.physical_id,c.material_id,c.region_id,c.geometry_id);
    }
    return result;
}

} // namespace OpenCAX

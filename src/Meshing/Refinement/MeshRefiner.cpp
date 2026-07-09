#include <OpenCAX/Meshing/Refinement/MeshRefiner.h>

#include <array>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace OpenCAX
{
namespace
{
struct EdgeKey
{
    int a=-1,b=-1;
    EdgeKey()=default;
    EdgeKey(int x,int y){ if(x<y){a=x;b=y;}else{a=y;b=x;} }
    bool operator==(const EdgeKey& o) const { return a==o.a && b==o.b; }
};
struct EdgeKeyHash
{
    std::size_t operator()(const EdgeKey& e) const
    {
        return std::hash<int>()(e.a) ^ (std::hash<int>()(e.b)+0x9e3779b97f4a7c15ULL);
    }
};
using EdgeSet=std::unordered_set<EdgeKey,EdgeKeyHash>;
using EdgeMid=std::unordered_map<EdgeKey,int,EdgeKeyHash>;

static void require_tri3(const TriangleMesh& mesh)
{
    for(const auto& c:mesh.cells())
        if(c.type!=CellType::Triangle3||c.node_ids.size()!=3)
            throw std::invalid_argument("MeshRefiner: only Triangle3 meshes are supported for now.");
}

static double dist2(const MeshNode& a,const MeshNode& b)
{
    const double dx=a.x-b.x,dy=a.y-b.y,dz=a.z-b.z; return dx*dx+dy*dy+dz*dz;
}

static EdgeKey longest_edge(const TriangleMesh& mesh,const MeshCell& c)
{
    int n0=c.node_ids[0],n1=c.node_ids[1],n2=c.node_ids[2];
    double d01=dist2(mesh.nodes()[n0],mesh.nodes()[n1]);
    double d12=dist2(mesh.nodes()[n1],mesh.nodes()[n2]);
    double d20=dist2(mesh.nodes()[n2],mesh.nodes()[n0]);
    if(d01>=d12&&d01>=d20)return EdgeKey(n0,n1);
    if(d12>=d01&&d12>=d20)return EdgeKey(n1,n2);
    return EdgeKey(n2,n0);
}

static int midpoint(const EdgeKey& e,std::vector<std::array<double,3>>& pts,EdgeMid& mids)
{
    auto it=mids.find(e); if(it!=mids.end())return it->second;
    auto p0=pts[e.a]; auto p1=pts[e.b];
    int id=static_cast<int>(pts.size());
    pts.push_back({0.5*(p0[0]+p1[0]),0.5*(p0[1]+p1[1]),0.5*(p0[2]+p1[2])});
    mids[e]=id; return id;
}

static double signed_area2(const std::vector<std::array<double,3>>& pts,int a,int b,int c)
{return (pts[b][0]-pts[a][0])*(pts[c][1]-pts[a][1])-(pts[b][1]-pts[a][1])*(pts[c][0]-pts[a][0]);}

static void add_tri(std::vector<std::array<int,3>>& tris,const std::vector<std::array<double,3>>& pts,int a,int b,int c)
{ if(signed_area2(pts,a,b,c)>=0) tris.push_back({a,b,c}); else tris.push_back({a,c,b}); }

static TriangleMesh refine_edges(const TriangleMesh& mesh,const EdgeSet& marked)
{
    require_tri3(mesh);
    std::vector<std::array<double,3>> pts;
    for(const auto& n:mesh.nodes()) pts.push_back({n.x,n.y,n.z});
    std::vector<std::array<int,3>> tris;
    EdgeMid mids;
    for(const auto& c:mesh.cells())
    {
        int n0=c.node_ids[0], n1=c.node_ids[1], n2=c.node_ids[2];
        EdgeKey e01(n0,n1), e12(n1,n2), e20(n2,n0);
        bool s01=marked.count(e01), s12=marked.count(e12), s20=marked.count(e20);
        int count=int(s01)+int(s12)+int(s20);
        if(count==0){add_tri(tris,pts,n0,n1,n2); continue;}
        int m01=s01?midpoint(e01,pts,mids):-1, m12=s12?midpoint(e12,pts,mids):-1, m20=s20?midpoint(e20,pts,mids):-1;
        if(count==1)
        {
            if(s01){add_tri(tris,pts,n0,m01,n2); add_tri(tris,pts,m01,n1,n2);} 
            else if(s12){add_tri(tris,pts,n1,m12,n0); add_tri(tris,pts,m12,n2,n0);} 
            else {add_tri(tris,pts,n2,m20,n1); add_tri(tris,pts,m20,n0,n1);} 
        }
        else if(count==2)
        {
            if(s01&&s12){add_tri(tris,pts,n1,m12,m01); add_tri(tris,pts,n0,m01,n2); add_tri(tris,pts,m01,m12,n2);} 
            else if(s12&&s20){add_tri(tris,pts,n2,m20,m12); add_tri(tris,pts,n1,m12,n0); add_tri(tris,pts,m12,m20,n0);} 
            else {add_tri(tris,pts,n0,m01,m20); add_tri(tris,pts,n2,m20,n1); add_tri(tris,pts,m20,m01,n1);} 
        }
        else
        {
            add_tri(tris,pts,n0,m01,m20); add_tri(tris,pts,m01,n1,m12); add_tri(tris,pts,m20,m12,n2); add_tri(tris,pts,m01,m12,m20);
        }
    }
    auto out=TriangleMesh::create_unstructured(pts,tris);
    out.set_name("RefinedTriangleMesh"); out.set_source("OpenCAX::Meshing::MeshRefiner"); out.set_source_type(MeshSourceType::Refined);
    return out;
}
}

TriangleMesh MeshRefiner::uniform_refine(const TriangleMesh& mesh,int levels)
{
    TriangleMesh cur=mesh;
    for(int l=0;l<levels;++l)
    {
        EdgeSet edges;
        for(const auto& c:cur.cells()){edges.insert(EdgeKey(c.node_ids[0],c.node_ids[1]));edges.insert(EdgeKey(c.node_ids[1],c.node_ids[2]));edges.insert(EdgeKey(c.node_ids[2],c.node_ids[0]));}
        cur=refine_edges(cur,edges);
    }
    return cur;
}

TriangleMesh MeshRefiner::bisection_refine(const TriangleMesh& mesh,int levels)
{
    TriangleMesh cur=mesh;
    for(int l=0;l<levels;++l)
    {
        EdgeSet edges; for(const auto& c:cur.cells()) edges.insert(longest_edge(cur,c));
        cur=refine_edges(cur,edges);
    }
    return cur;
}

TriangleMesh MeshRefiner::bisection_refine_marked_cells(const TriangleMesh& mesh,const std::vector<int>& ids)
{
    EdgeSet edges;
    for(int id:ids){ if(!mesh.valid_cell_id(id)) throw std::out_of_range("MeshRefiner::bisection_refine_marked_cells: invalid cell id."); edges.insert(longest_edge(mesh,mesh.cells()[id])); }
    return refine_edges(mesh,edges);
}

} // namespace OpenCAX

#include <OpenCAX/Meshing/Quality/MeshQuality.h>
#include <OpenCAX/Mesh2/CellTopology.h>
#include <OpenCAX/Mesh2/TriangleMesh.h>
#include <OpenCAX/Mesh2/TetraMesh.h>
#include <OpenCAX/Mesh2/HexMesh.h>

#include <algorithm>
#include <limits>

namespace OpenCAX
{

double MeshQuality::triangle_area(const Mesh& mesh,int cell_id)
{
    const auto& c=mesh.cells()[cell_id]; if(c.node_ids.size()<3) return 0.0;
    return TriangleMesh::triangle_area(mesh.nodes()[c.node_ids[0]],mesh.nodes()[c.node_ids[1]],mesh.nodes()[c.node_ids[2]]);
}

double MeshQuality::tetra_volume(const Mesh& mesh,int cell_id)
{
    const auto& c=mesh.cells()[cell_id]; if(c.node_ids.size()<4) return 0.0;
    return TetraMesh::tetra_volume(mesh.nodes()[c.node_ids[0]],mesh.nodes()[c.node_ids[1]],mesh.nodes()[c.node_ids[2]],mesh.nodes()[c.node_ids[3]]);
}

double MeshQuality::hexa_volume(const Mesh& mesh,int cell_id)
{
    const auto& c=mesh.cells()[cell_id]; if(c.node_ids.size()<8) return 0.0;
    return HexMesh::hexa_volume(mesh.nodes()[c.node_ids[0]],mesh.nodes()[c.node_ids[1]],mesh.nodes()[c.node_ids[2]],mesh.nodes()[c.node_ids[3]],mesh.nodes()[c.node_ids[4]],mesh.nodes()[c.node_ids[5]],mesh.nodes()[c.node_ids[6]],mesh.nodes()[c.node_ids[7]]);
}

double MeshQuality::cell_measure(const Mesh& mesh,int cell_id)
{
    if(!mesh.valid_cell_id(cell_id)) return 0.0;
    const auto type=mesh.cells()[cell_id].type;
    switch(CellTopology::family(type))
    {
    case MeshElementFamily::Triangle: return triangle_area(mesh,cell_id);
    case MeshElementFamily::Tetrahedron: return tetra_volume(mesh,cell_id);
    case MeshElementFamily::Hexahedron: return hexa_volume(mesh,cell_id);
    default: return 0.0;
    }
}

MeshQualitySummary MeshQuality::summarize(const Mesh& mesh,double eps)
{
    MeshQualitySummary s;
    if(mesh.num_cells()==0) return s;
    s.valid=true;
    s.min_measure=std::numeric_limits<double>::max();
    s.max_measure=std::numeric_limits<double>::lowest();
    double sum=0.0;
    for(std::size_t i=0;i<mesh.num_cells();++i)
    {
        double m=cell_measure(mesh,static_cast<int>(i));
        s.min_measure=std::min(s.min_measure,m); s.max_measure=std::max(s.max_measure,m); sum+=m;
        if(m<=eps){s.valid=false; ++s.bad_cell_count;}
    }
    s.avg_measure=sum/static_cast<double>(mesh.num_cells());
    return s;
}

} // namespace OpenCAX

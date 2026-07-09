#include <OpenCAX/Meshing/Gmsh/GmshMeshReader.h>
#include <OpenCAX/Meshing/Gmsh/GmshElementMap.h>
#include <OpenCAX/Mesh2/CellTopology.h>

#ifdef OPEN_CAX_HAS_GMSH
#include <gmsh.h>
#endif

#include <stdexcept>
#include <unordered_map>

namespace OpenCAX
{

Mesh GmshMeshReader::read_mesh(const std::string& filename)
{
#ifndef OPEN_CAX_HAS_GMSH
    (void)filename;
    throw std::runtime_error("GmshMeshReader requires OPEN_CAX_HAS_GMSH.");
#else
    if(!gmsh::isInitialized()) gmsh::initialize();
    gmsh::open(filename);

    std::vector<std::size_t> node_tags;
    std::vector<double> coords, params;
    gmsh::model::mesh::getNodes(node_tags, coords, params);

    Mesh mesh;
    mesh.set_name("GmshImportedMesh");
    mesh.set_source(filename);
    mesh.set_source_type(MeshSourceType::Gmsh);

    std::unordered_map<std::size_t,int> tag_to_id;
    for(std::size_t i=0;i<node_tags.size();++i)
    {
        int id=mesh.add_node(coords[3*i],coords[3*i+1],coords[3*i+2]);
        tag_to_id[node_tags[i]]=id;
    }

    std::vector<int> element_types;
    std::vector<std::vector<std::size_t>> element_tags, element_node_tags;
    gmsh::model::mesh::getElements(element_types, element_tags, element_node_tags);

    MeshDimension max_dim=MeshDimension::Unknown;
    for(std::size_t b=0;b<element_types.size();++b)
    {
        CellType ct=GmshElementMap::to_opencax(element_types[b]);
        int n=CellTopology::num_nodes(ct);
        if(ct==CellType::Unknown||n<=0) continue;
        auto dim=CellTopology::dimension(ct);
        if(static_cast<int>(dim)>static_cast<int>(max_dim)) max_dim=dim;
        const auto& conn=element_node_tags[b];
        for(std::size_t i=0;i+n<=conn.size();i+=n)
        {
            std::vector<int> ids;
            for(int k=0;k<n;++k) ids.push_back(tag_to_id.at(conn[i+k]));
            mesh.add_cell(ct,GmshElementMap::reorder_to_opencax(ct,ids));
        }
    }
    mesh.set_dimension(max_dim);
    return mesh;
#endif
}

TriangleMesh GmshMeshReader::read_triangle_mesh(const std::string& filename)
{
    Mesh base=read_mesh(filename);
    TriangleMesh mesh;
    mesh.info()=base.info();
    for(const auto& n:base.nodes()) mesh.add_node(n.x,n.y,n.z,n.physical_id,n.boundary_id,n.geometry_id);
    for(const auto& c:base.cells())
        if(c.type==CellType::Triangle3 && c.node_ids.size()==3)
            mesh.add_cell(CellType::Triangle3,c.node_ids,c.physical_id,c.material_id,c.region_id,c.geometry_id);
        else if(c.type==CellType::Triangle6 && c.node_ids.size()==6)
            mesh.add_cell(CellType::Triangle6,c.node_ids,c.physical_id,c.material_id,c.region_id,c.geometry_id);
    return mesh;
}

TetraMesh GmshMeshReader::read_tetra_mesh(const std::string& filename)
{
    Mesh base=read_mesh(filename);
    TetraMesh mesh;
    mesh.info()=base.info();
    for(const auto& n:base.nodes()) mesh.add_node(n.x,n.y,n.z,n.physical_id,n.boundary_id,n.geometry_id);
    for(const auto& c:base.cells())
        if((c.type==CellType::Tetra4 || c.type==CellType::Tetra10)) mesh.add_cell(c.type,c.node_ids,c.physical_id,c.material_id,c.region_id,c.geometry_id);
    return mesh;
}

} // namespace OpenCAX

#include <OpenCAX/Mesh/MeshDebugUtils.h>
#include <OpenCAX/Mesh/CellTopology.h>

#include <iostream>
#include <ostream>

namespace OpenCAX
{

void MeshDebugUtils::print_vector_int(const std::vector<int>& v, std::ostream& os)
{
    os << "[";
    for (std::size_t i=0;i<v.size();++i) { os << v[i]; if (i+1<v.size()) os << ", "; }
    os << "]";
}
void MeshDebugUtils::print_vector_int(const std::vector<int>& v){ print_vector_int(v, std::cout); }

void MeshDebugUtils::print_mesh(const Mesh& mesh, std::ostream& os)
{
    os << "\n===== MESH =====\n";
    os << "name  = " << mesh.info().name << "\n";
    os << "nodes = " << mesh.num_nodes() << "\n";
    os << "cells = " << mesh.num_cells() << "\n";

    os << "\n--- nodes ---\n";
    for (const auto& n : mesh.nodes()) os << n.id << ": [" << n.x << ", " << n.y << ", " << n.z << "]\n";

    os << "\n--- cells ---\n";
    for (const auto& c : mesh.cells())
    {
        os << c.id << ": type=" << CellTopology::to_string(c.type) << " nodes=";
        print_vector_int(c.node_ids, os);
        os << "\n";
    }
}
void MeshDebugUtils::print_mesh(const Mesh& mesh){ print_mesh(mesh, std::cout); }

void MeshDebugUtils::print_topology(const MeshTopology& topo, std::ostream& os)
{
    os << "\n===== TOPOLOGY =====\n";
    os << "edges = " << topo.num_edges() << "\n";
    os << "faces = " << topo.num_faces() << "\n";

    os << "\n--- edges ---\n";
    for(std::size_t i=0;i<topo.edges().size();++i) os << i << ": [" << topo.edges()[i][0] << ", " << topo.edges()[i][1] << "]\n";

    auto dump = [&](const char* name, const std::vector<std::vector<int>>& table)
    {
        os << "\n--- " << name << " ---\n";
        for(std::size_t i=0;i<table.size();++i){ os << i << ": "; print_vector_int(table[i], os); os << "\n"; }
    };
    dump("cell2edge", topo.cell2edge());
    dump("edge2cell", topo.edge2cell());
    dump("node2cell", topo.node2cell());
    dump("node2node", topo.node2node());
    dump("cell2cell", topo.cell2cell());

    os << "\n--- boundary ---\n";
    os << "boundary_edges = "; print_vector_int(topo.boundary_edges(), os); os << "\n";
    os << "boundary_faces = "; print_vector_int(topo.boundary_faces(), os); os << "\n";
    os << "boundary_nodes = "; print_vector_int(topo.boundary_nodes(), os); os << "\n";
    os << "boundary_cells = "; print_vector_int(topo.boundary_cells(), os); os << "\n";
}
void MeshDebugUtils::print_topology(const MeshTopology& topo){ print_topology(topo, std::cout); }

bool MeshDebugUtils::check_mesh(const Mesh& mesh, std::ostream& os)
{
    std::string error;
    const bool ok = mesh.validate(&error);
    os << (ok ? "[MeshDebugUtils] mesh valid.\n" : "[MeshDebugUtils] mesh invalid: ") << (ok ? "" : error) << "\n";
    return ok;
}

bool MeshDebugUtils::check_mesh(const Mesh& mesh){ return check_mesh(mesh, std::cout); }

} // namespace OpenCAX

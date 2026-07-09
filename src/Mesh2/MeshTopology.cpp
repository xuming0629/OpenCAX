#include <OpenCAX/Mesh2/MeshTopology.h>
#include <OpenCAX/Mesh2/CellTopology.h>

#include <algorithm>
#include <map>
#include <set>

namespace OpenCAX
{

void MeshTopology::clear()
{
    built_ = false;
    level_ = TopologyLevel::None;
    edges_.clear(); faces_.clear(); face_types_.clear();
    cell2edge_.clear(); cell2face_.clear(); edge2cell_.clear(); face2cell_.clear();
    node2edge_.clear(); node2face_.clear(); node2cell_.clear(); node2node_.clear();
    edge2face_.clear(); face2edge_.clear(); cell2cell_.clear();
    boundary_edges_.clear(); boundary_faces_.clear(); boundary_nodes_.clear(); boundary_cells_.clear();
}

bool MeshTopology::built() const { return built_; }
std::size_t MeshTopology::num_edges() const { return edges_.size(); }
std::size_t MeshTopology::num_faces() const { return faces_.size(); }
const std::vector<std::array<int, 2>>& MeshTopology::edges() const { return edges_; }
const std::vector<std::vector<int>>& MeshTopology::faces() const { return faces_; }
const std::vector<CellType>& MeshTopology::face_types() const { return face_types_; }
const std::vector<std::vector<int>>& MeshTopology::cell2edge() const { return cell2edge_; }
const std::vector<std::vector<int>>& MeshTopology::cell2face() const { return cell2face_; }
const std::vector<std::vector<int>>& MeshTopology::edge2cell() const { return edge2cell_; }
const std::vector<std::vector<int>>& MeshTopology::face2cell() const { return face2cell_; }
const std::vector<std::vector<int>>& MeshTopology::node2edge() const { return node2edge_; }
const std::vector<std::vector<int>>& MeshTopology::node2face() const { return node2face_; }
const std::vector<std::vector<int>>& MeshTopology::node2cell() const { return node2cell_; }
const std::vector<std::vector<int>>& MeshTopology::node2node() const { return node2node_; }
const std::vector<std::vector<int>>& MeshTopology::edge2face() const { return edge2face_; }
const std::vector<std::vector<int>>& MeshTopology::face2edge() const { return face2edge_; }
const std::vector<std::vector<int>>& MeshTopology::cell2cell() const { return cell2cell_; }
const std::vector<int>& MeshTopology::boundary_edges() const { return boundary_edges_; }
const std::vector<int>& MeshTopology::boundary_faces() const { return boundary_faces_; }
const std::vector<int>& MeshTopology::boundary_nodes() const { return boundary_nodes_; }
const std::vector<int>& MeshTopology::boundary_cells() const { return boundary_cells_; }

std::array<int,2> MeshTopology::canonical_edge(int a,int b){return a<=b?std::array<int,2>{a,b}:std::array<int,2>{b,a};}
std::vector<int> MeshTopology::canonical_face(const std::vector<int>& ids){auto r=ids; std::sort(r.begin(),r.end()); return r;}
void MeshTopology::sort_unique(std::vector<int>& v){std::sort(v.begin(),v.end()); v.erase(std::unique(v.begin(),v.end()),v.end());}

void MeshTopology::build(const Mesh& mesh, TopologyLevel level)
{
    clear();
    level_ = level;
    if (level == TopologyLevel::None) { built_ = true; return; }

    const auto& nodes = mesh.nodes();
    const auto& cells = mesh.cells();
    const std::size_t node_count = nodes.size();
    const std::size_t cell_count = cells.size();

    node2edge_.resize(node_count);
    node2face_.resize(node_count);
    node2cell_.resize(node_count);
    node2node_.resize(node_count);
    cell2edge_.resize(cell_count);
    cell2face_.resize(cell_count);
    cell2cell_.resize(cell_count);

    std::map<std::array<int,2>, int> edge_map;
    std::map<std::vector<int>, int> face_map;
    std::map<int, std::set<int>> face_edge_accumulator;

    for (std::size_t ci=0; ci<cell_count; ++ci)
    {
        const auto& cell = cells[ci];
        const int cell_id = static_cast<int>(ci);

        for (int node_id : cell.node_ids)
        {
            if (node_id >= 0 && static_cast<std::size_t>(node_id) < node_count)
                node2cell_[node_id].push_back(cell_id);
        }

        const auto local_edges = CellTopology::topology_edges(cell.type, cell.node_ids);
        for (const auto& raw_edge : local_edges)
        {
            const auto e = canonical_edge(raw_edge[0], raw_edge[1]);
            int edge_id = -1;
            auto it = edge_map.find(e);
            if (it == edge_map.end())
            {
                edge_id = static_cast<int>(edges_.size());
                edge_map[e] = edge_id;
                edges_.push_back(e);
                edge2cell_.push_back({});
            }
            else edge_id = it->second;

            cell2edge_[cell_id].push_back(edge_id);
            edge2cell_[edge_id].push_back(cell_id);

            const int a=e[0], b=e[1];
            if (a>=0 && static_cast<std::size_t>(a)<node_count) { node2edge_[a].push_back(edge_id); node2node_[a].push_back(b); }
            if (b>=0 && static_cast<std::size_t>(b)<node_count) { node2edge_[b].push_back(edge_id); node2node_[b].push_back(a); }
        }

        if (level == TopologyLevel::Edges) continue;

        const auto local_faces = CellTopology::faces(cell.type, cell.node_ids);
        const auto local_face_types = CellTopology::face_types(cell.type);
        for (std::size_t lf=0; lf<local_faces.size(); ++lf)
        {
            const auto& raw_face = local_faces[lf];
            if (raw_face.size()<3) continue;
            const auto key = canonical_face(raw_face);
            int face_id = -1;
            auto it = face_map.find(key);
            if (it == face_map.end())
            {
                face_id = static_cast<int>(faces_.size());
                face_map[key] = face_id;
                faces_.push_back(raw_face);
                face2cell_.push_back({});
                face_types_.push_back(lf < local_face_types.size() ? local_face_types[lf] : CellType::Unknown);
            }
            else face_id = it->second;

            cell2face_[cell_id].push_back(face_id);
            face2cell_[face_id].push_back(cell_id);

            for (std::size_t k=0;k<raw_face.size();++k)
            {
                const auto fe = canonical_edge(raw_face[k], raw_face[(k+1)%raw_face.size()]);
                auto eit = edge_map.find(fe);
                if (eit != edge_map.end()) face_edge_accumulator[face_id].insert(eit->second);
            }

            for (int node_id : raw_face)
                if (node_id>=0 && static_cast<std::size_t>(node_id)<node_count) node2face_[node_id].push_back(face_id);
        }
    }

    edge2face_.resize(edges_.size());
    face2edge_.resize(faces_.size());
    for (const auto& item : face_edge_accumulator)
    {
        const int face_id = item.first;
        if (face_id < 0 || static_cast<std::size_t>(face_id) >= face2edge_.size()) continue;
        for (int edge_id : item.second)
        {
            if (edge_id < 0 || static_cast<std::size_t>(edge_id) >= edge2face_.size()) continue;
            face2edge_[face_id].push_back(edge_id);
            edge2face_[edge_id].push_back(face_id);
        }
    }

    std::set<int> boundary_edge_set, boundary_face_set, boundary_node_set, boundary_cell_set;
    if (!faces_.empty())
    {
        for (std::size_t fi=0; fi<face2cell_.size(); ++fi)
        {
            if (face2cell_[fi].size() == 1)
            {
                const int face_id = static_cast<int>(fi);
                const int cell_id = face2cell_[fi][0];
                boundary_face_set.insert(face_id);
                boundary_cell_set.insert(cell_id);
                for (int nid : faces_[fi]) boundary_node_set.insert(nid);
                if (fi < face2edge_.size()) for (int eid : face2edge_[fi]) boundary_edge_set.insert(eid);
            }
        }
    }
    else
    {
        for (std::size_t ei=0; ei<edge2cell_.size(); ++ei)
        {
            if (edge2cell_[ei].size() == 1)
            {
                boundary_edge_set.insert(static_cast<int>(ei));
                boundary_cell_set.insert(edge2cell_[ei][0]);
                boundary_node_set.insert(edges_[ei][0]);
                boundary_node_set.insert(edges_[ei][1]);
            }
        }
    }

    boundary_edges_.assign(boundary_edge_set.begin(), boundary_edge_set.end());
    boundary_faces_.assign(boundary_face_set.begin(), boundary_face_set.end());
    boundary_nodes_.assign(boundary_node_set.begin(), boundary_node_set.end());
    boundary_cells_.assign(boundary_cell_set.begin(), boundary_cell_set.end());

    if (level == TopologyLevel::Full)
    {
        if (!faces_.empty())
        {
            for (const auto& adj : face2cell_) if (adj.size() >= 2)
                for (std::size_t i=0;i<adj.size();++i) for (std::size_t j=i+1;j<adj.size();++j)
                { cell2cell_[adj[i]].push_back(adj[j]); cell2cell_[adj[j]].push_back(adj[i]); }
        }
        else
        {
            for (const auto& adj : edge2cell_) if (adj.size() >= 2)
                for (std::size_t i=0;i<adj.size();++i) for (std::size_t j=i+1;j<adj.size();++j)
                { cell2cell_[adj[i]].push_back(adj[j]); cell2cell_[adj[j]].push_back(adj[i]); }
        }
    }

    for(auto& v: cell2edge_) sort_unique(v);
    for(auto& v: cell2face_) sort_unique(v);
    for(auto& v: edge2cell_) sort_unique(v);
    for(auto& v: face2cell_) sort_unique(v);
    for(auto& v: node2edge_) sort_unique(v);
    for(auto& v: node2face_) sort_unique(v);
    for(auto& v: node2cell_) sort_unique(v);
    for(auto& v: node2node_) sort_unique(v);
    for(auto& v: edge2face_) sort_unique(v);
    for(auto& v: face2edge_) sort_unique(v);
    for(auto& v: cell2cell_) sort_unique(v);
    sort_unique(boundary_edges_); sort_unique(boundary_faces_); sort_unique(boundary_nodes_); sort_unique(boundary_cells_);

    built_ = true;
}

} // namespace OpenCAX

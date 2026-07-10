/**
 * @file MeshTopology.cpp
 * @brief OpenCAX Mesh2 全局拓扑关系构建实现
 *
 * MeshTopology 负责从 Mesh 数据中构造全局拓扑。
 *
 * Mesh:
 *
 *     nodes
 *     cells
 *
 * MeshTopology:
 *
 *     edges
 *     faces
 *     adjacency
 *     boundary
 *
 *
 * 支持:
 *
 * 2D:
 *
 * Triangle
 * Quad
 *
 *
 * 3D:
 *
 * Tetra
 * Hexa
 * Prism
 * Pyramid
 *
 *
 * 高阶单元:
 *
 * Triangle6
 * Quad8
 * Quad9
 * Tetra10
 * Hexa20
 * Hexa27
 *
 * 默认使用角点拓扑:
 *
 * Triangle6:
 *
 * node:
 *
 *     0
 *    / \
 *   5   4
 *  /     \
 * 1---3---2
 *
 *
 * topology edge:
 *
 * (0,1)
 * (1,2)
 * (2,0)
 *
 *
 * 中间节点用于:
 *
 * FEM
 * 插值
 * VTK quadratic cell
 *
 * 不参与基础拓扑。
 */


#include <OpenCAX/Mesh2/MeshTopology.h>
#include <OpenCAX/Mesh2/CellTopology.h>

#include <algorithm>
#include <map>
#include <set>


namespace OpenCAX
{


// ============================================================
// 清空拓扑
// ============================================================

void MeshTopology::clear()
{
    built_ = false;

    level_ =
        TopologyLevel::None;


    edges_.clear();
    faces_.clear();
    face_types_.clear();


    cell2edge_.clear();
    cell2face_.clear();

    edge2cell_.clear();
    face2cell_.clear();


    node2edge_.clear();
    node2face_.clear();
    node2cell_.clear();
    node2node_.clear();


    edge2face_.clear();
    face2edge_.clear();

    cell2cell_.clear();


    boundary_edges_.clear();
    boundary_faces_.clear();
    boundary_nodes_.clear();
    boundary_cells_.clear();
}



// ============================================================
// Getter
// ============================================================


bool MeshTopology::built() const
{
    return built_;
}


std::size_t MeshTopology::num_edges() const
{
    return edges_.size();
}


std::size_t MeshTopology::num_faces() const
{
    return faces_.size();
}


const std::vector<std::array<int,2>>&
MeshTopology::edges() const
{
    return edges_;
}


const std::vector<std::vector<int>>&
MeshTopology::faces() const
{
    return faces_;
}


const std::vector<CellType>&
MeshTopology::face_types() const
{
    return face_types_;
}


const std::vector<std::vector<int>>&
MeshTopology::cell2edge() const
{
    return cell2edge_;
}


const std::vector<std::vector<int>>&
MeshTopology::cell2face() const
{
    return cell2face_;
}


const std::vector<std::vector<int>>&
MeshTopology::edge2cell() const
{
    return edge2cell_;
}


const std::vector<std::vector<int>>&
MeshTopology::face2cell() const
{
    return face2cell_;
}


const std::vector<std::vector<int>>&
MeshTopology::node2edge() const
{
    return node2edge_;
}


const std::vector<std::vector<int>>&
MeshTopology::node2face() const
{
    return node2face_;
}


const std::vector<std::vector<int>>&
MeshTopology::node2cell() const
{
    return node2cell_;
}


const std::vector<std::vector<int>>&
MeshTopology::node2node() const
{
    return node2node_;
}


const std::vector<std::vector<int>>&
MeshTopology::cell2cell() const
{
    return cell2cell_;
}


const std::vector<int>&
MeshTopology::boundary_edges() const
{
    return boundary_edges_;
}


const std::vector<int>&
MeshTopology::boundary_faces() const
{
    return boundary_faces_;
}


const std::vector<int>&
MeshTopology::boundary_nodes() const
{
    return boundary_nodes_;
}


const std::vector<int>&
MeshTopology::boundary_cells() const
{
    return boundary_cells_;
}



// ============================================================
// 辅助函数
// ============================================================


std::array<int,2>
MeshTopology::canonical_edge(
    int a,
    int b
)
{
    if(a < b)
        return {a,b};

    return {b,a};
}



std::vector<int>
MeshTopology::canonical_face(
    const std::vector<int>& ids
)
{
    auto result = ids;

    std::sort(
        result.begin(),
        result.end()
    );

    return result;
}



void MeshTopology::sort_unique(
    std::vector<int>& values
)
{
    std::sort(
        values.begin(),
        values.end()
    );

    values.erase(
        std::unique(
            values.begin(),
            values.end()
        ),
        values.end()
    );
}



// ============================================================
// 核心拓扑构建
// ============================================================


void MeshTopology::build(
    const Mesh& mesh,
    TopologyLevel level
)
{

    clear();


    level_ = level;


    if(level == TopologyLevel::None)
    {
        built_ = true;
        return;
    }



    const auto& nodes =
        mesh.nodes();


    const auto& cells =
        mesh.cells();



    const std::size_t node_count =
        nodes.size();


    const std::size_t cell_count =
        cells.size();



    // -----------------------------
    // 初始化邻接结构
    // -----------------------------

    node2edge_.resize(node_count);
    node2face_.resize(node_count);
    node2cell_.resize(node_count);
    node2node_.resize(node_count);


    cell2edge_.resize(cell_count);
    cell2face_.resize(cell_count);
    cell2cell_.resize(cell_count);



    std::map<
        std::array<int,2>,
        int
    > edge_map;



    std::map<
        std::vector<int>,
        int
    > face_map;



    // =====================================================
    // 第一阶段:
    // 遍历 Cell
    //
    // 建立:
    //
    // cell2edge
    // edge2cell
    // node2edge
    // node2cell
    // node2node
    //
    // =====================================================


    for(std::size_t cid=0;
        cid<cell_count;
        ++cid)
    {

        const auto& cell =
            cells[cid];


        const int cell_id =
            static_cast<int>(cid);



        // node -> cell

        for(int nid:cell.node_ids)
        {
            node2cell_[nid]
                .push_back(cell_id);
        }




        // =========================
        // Edge
        // =========================


        auto local_edges =
            CellTopology::topology_edges(
                cell.type,
                cell.node_ids
            );



        for(auto e0:local_edges)
        {

            auto e =
                canonical_edge(
                    e0[0],
                    e0[1]
                );


            int eid;


            auto iter =
                edge_map.find(e);



            if(iter == edge_map.end())
            {

                eid =
                    static_cast<int>(
                        edges_.size()
                    );


                edge_map[e]=eid;


                edges_.push_back(e);


                edge2cell_
                    .push_back({});
            }
            else
            {
                eid =
                    iter->second;
            }



            cell2edge_[cell_id]
                .push_back(eid);


            edge2cell_[eid]
                .push_back(cell_id);



            node2edge_[e[0]]
                .push_back(eid);


            node2edge_[e[1]]
                .push_back(eid);



            node2node_[e[0]]
                .push_back(e[1]);


            node2node_[e[1]]
                .push_back(e[0]);

        }



        if(level == TopologyLevel::Edges)
            continue;



        // =================================================
        // Face
        // =================================================


        auto local_faces =
            CellTopology::faces(
                cell.type,
                cell.node_ids
            );


        auto local_face_types =
            CellTopology::face_types(
                cell.type
            );



        for(std::size_t fi=0;
            fi<local_faces.size();
            ++fi)
        {

            auto key =
                canonical_face(
                    local_faces[fi]
                );


            int face_id;



            auto iter =
                face_map.find(key);



            if(iter == face_map.end())
            {

                face_id =
                    static_cast<int>(
                        faces_.size()
                    );


                face_map[key]=face_id;


                faces_.push_back(
                    local_faces[fi]
                );


                face2cell_
                    .push_back({});


                face_types_
                    .push_back(
                        local_face_types[fi]
                    );

            }
            else
            {
                face_id =
                    iter->second;
            }



            cell2face_[cell_id]
                .push_back(face_id);



            face2cell_[face_id]
                .push_back(cell_id);



            for(int nid:local_faces[fi])
            {
                node2face_[nid]
                    .push_back(face_id);
            }

        }

    }



    // =====================================================
    // Boundary 判断
    // =====================================================


    std::set<int> bnodes;
    std::set<int> bedges;
    std::set<int> bfaces;
    std::set<int> bcells;



    if(!faces_.empty())
    {

        for(std::size_t i=0;
            i<face2cell_.size();
            ++i)
        {

            if(face2cell_[i].size()==1)
            {

                bfaces.insert(i);


                int cid =
                    face2cell_[i][0];


                bcells.insert(cid);



                for(int nid:faces_[i])
                    bnodes.insert(nid);

            }

        }

    }
    else
    {

        for(std::size_t i=0;
            i<edge2cell_.size();
            ++i)
        {

            if(edge2cell_[i].size()==1)
            {

                bedges.insert(i);


                bcells.insert(
                    edge2cell_[i][0]
                );


                bnodes.insert(
                    edges_[i][0]
                );


                bnodes.insert(
                    edges_[i][1]
                );
            }
        }
    }



    boundary_nodes_ =
        std::vector<int>(
            bnodes.begin(),
            bnodes.end()
        );


    boundary_edges_ =
        std::vector<int>(
            bedges.begin(),
            bedges.end()
        );


    boundary_faces_ =
        std::vector<int>(
            bfaces.begin(),
            bfaces.end()
        );


    boundary_cells_ =
        std::vector<int>(
            bcells.begin(),
            bcells.end()
        );




    // =====================================================
    // Cell 邻接
    //
    // 共享 edge/face
    //
    // =====================================================


    if(level == TopologyLevel::Full)
    {

        if(!faces_.empty())
        {

            for(auto& fc:face2cell_)
            {

                if(fc.size()<2)
                    continue;


                for(std::size_t i=0;i<fc.size();++i)
                {
                    for(std::size_t j=i+1;j<fc.size();++j)
                    {
                        cell2cell_[fc[i]]
                            .push_back(fc[j]);

                        cell2cell_[fc[j]]
                            .push_back(fc[i]);
                    }
                }

            }

        }
        else
        {

            for(auto& ec:edge2cell_)
            {

                if(ec.size()<2)
                    continue;


                for(std::size_t i=0;i<ec.size();++i)
                {
                    for(std::size_t j=i+1;j<ec.size();++j)
                    {
                        cell2cell_[ec[i]]
                            .push_back(ec[j]);

                        cell2cell_[ec[j]]
                            .push_back(ec[i]);
                    }
                }
            }
        }

    }




    // =====================================================
    // 去重
    // =====================================================


    for(auto& v:node2edge_)
        sort_unique(v);


    for(auto& v:node2face_)
        sort_unique(v);


    for(auto& v:node2cell_)
        sort_unique(v);


    for(auto& v:node2node_)
        sort_unique(v);



    for(auto& v:cell2edge_)
        sort_unique(v);


    for(auto& v:cell2face_)
        sort_unique(v);


    for(auto& v:edge2cell_)
        sort_unique(v);


    for(auto& v:face2cell_)
        sort_unique(v);


    for(auto& v:cell2cell_)
        sort_unique(v);



    built_ = true;

}



} // namespace OpenCAX
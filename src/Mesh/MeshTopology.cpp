/**
 * @file MeshTopology.cpp
 * @brief OpenCAX 网格全局拓扑关系构建器实现
 *
 * 本文件实现 MeshTopology 类。
 *
 * 主要构建内容包括：
 *
 * - 全局边
 * - 全局面
 * - 单元到边
 * - 单元到面
 * - 边到单元
 * - 面到单元
 * - 节点到边
 * - 节点到面
 * - 节点到单元
 * - 节点到相邻节点
 * - 边到面
 * - 面到边
 * - 单元到相邻单元
 * - 边界边
 * - 边界面
 * - 边界节点
 * - 边界单元
 */

#include <OpenCAX/Mesh/MeshTopology.h>
#include <OpenCAX/Mesh/CellTopology.h>

#include <algorithm>
#include <map>
#include <set>

namespace OpenCAX
{

void MeshTopology::clear()
{
    built_ = false;

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

const std::vector<std::array<int, 2>>& MeshTopology::edges() const
{
    return edges_;
}

const std::vector<std::vector<int>>& MeshTopology::faces() const
{
    return faces_;
}

const std::vector<CellType>& MeshTopology::face_types() const
{
    return face_types_;
}

const std::vector<std::vector<int>>& MeshTopology::cell2edge() const
{
    return cell2edge_;
}

const std::vector<std::vector<int>>& MeshTopology::cell2face() const
{
    return cell2face_;
}

const std::vector<std::vector<int>>& MeshTopology::edge2cell() const
{
    return edge2cell_;
}

const std::vector<std::vector<int>>& MeshTopology::face2cell() const
{
    return face2cell_;
}

const std::vector<std::vector<int>>& MeshTopology::node2edge() const
{
    return node2edge_;
}

const std::vector<std::vector<int>>& MeshTopology::node2face() const
{
    return node2face_;
}

const std::vector<std::vector<int>>& MeshTopology::node2cell() const
{
    return node2cell_;
}

const std::vector<std::vector<int>>& MeshTopology::node2node() const
{
    return node2node_;
}

const std::vector<std::vector<int>>& MeshTopology::edge2face() const
{
    return edge2face_;
}

const std::vector<std::vector<int>>& MeshTopology::face2edge() const
{
    return face2edge_;
}

const std::vector<std::vector<int>>& MeshTopology::cell2cell() const
{
    return cell2cell_;
}

const std::vector<int>& MeshTopology::boundary_edges() const
{
    return boundary_edges_;
}

const std::vector<int>& MeshTopology::boundary_faces() const
{
    return boundary_faces_;
}

const std::vector<int>& MeshTopology::boundary_nodes() const
{
    return boundary_nodes_;
}

const std::vector<int>& MeshTopology::boundary_cells() const
{
    return boundary_cells_;
}

std::array<int, 2> MeshTopology::canonical_edge(
    int a,
    int b
)
{
    if (a <= b)
    {
        return {a, b};
    }

    return {b, a};
}

std::vector<int> MeshTopology::canonical_face(
    const std::vector<int>& node_ids
)
{
    std::vector<int> result = node_ids;

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

void MeshTopology::build(
    const Mesh& mesh
)
{
    clear();

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

    std::map<std::array<int, 2>, int> edge_map;
    std::map<std::vector<int>, int> face_map;

    /*
     * 面到边临时累积表。
     *
     * 使用 set 是为了避免重复插入。
     * 最终会转换为 face2edge_。
     */
    std::map<int, std::set<int>> face_edge_accumulator;

    /*
     * 第一阶段：
     *
     * 遍历单元，构建：
     *
     * - edges_
     * - faces_
     * - cell2edge_
     * - cell2face_
     * - edge2cell_
     * - face2cell_
     * - node2edge_
     * - node2face_
     * - node2cell_
     * - node2node_
     */
    for (std::size_t ci = 0; ci < cell_count; ++ci)
    {
        const MeshCell& cell = cells[ci];
        const int cell_id = static_cast<int>(ci);

        /*
         * node2cell
         */
        for (int node_id : cell.node_ids)
        {
            if (node_id >= 0 &&
                static_cast<std::size_t>(node_id) < node_count)
            {
                node2cell_[static_cast<std::size_t>(node_id)].push_back(cell_id);
            }
        }

        /*
         * 单元局部边。
         *
         * Triangle3:
         * - 3 条边
         *
         * Quad4:
         * - 4 条边
         *
         * Tetra4:
         * - 6 条边
         *
         * Hexa8:
         * - 12 条边
         */
        const auto local_edges = CellTopology::edges(
            cell.type,
            cell.node_ids
        );

        for (const auto& raw_edge : local_edges)
        {
            const int raw_a = raw_edge[0];
            const int raw_b = raw_edge[1];

            const auto e = canonical_edge(
                raw_a,
                raw_b
            );

            int edge_id = -1;

            auto it = edge_map.find(e);

            if (it == edge_map.end())
            {
                edge_id = static_cast<int>(edges_.size());

                edge_map[e] = edge_id;
                edges_.push_back(e);
                edge2cell_.push_back({});
            }
            else
            {
                edge_id = it->second;
            }

            cell2edge_[static_cast<std::size_t>(cell_id)].push_back(edge_id);
            edge2cell_[static_cast<std::size_t>(edge_id)].push_back(cell_id);

            const int a = e[0];
            const int b = e[1];

            if (a >= 0 && static_cast<std::size_t>(a) < node_count)
            {
                node2edge_[static_cast<std::size_t>(a)].push_back(edge_id);
                node2node_[static_cast<std::size_t>(a)].push_back(b);
            }

            if (b >= 0 && static_cast<std::size_t>(b) < node_count)
            {
                node2edge_[static_cast<std::size_t>(b)].push_back(edge_id);
                node2node_[static_cast<std::size_t>(b)].push_back(a);
            }
        }

        /*
         * 单元局部面。
         *
         * 对 2D Triangle / Quad：
         *
         * - CellTopology::faces 通常返回空。
         *
         * 对 3D Tetra / Hexa：
         *
         * - 返回局部面节点列表。
         */
        const auto local_faces = CellTopology::faces(
            cell.type,
            cell.node_ids
        );

        const auto local_face_types = CellTopology::face_types(
            cell.type
        );

        for (std::size_t lf = 0; lf < local_faces.size(); ++lf)
        {
            const auto& raw_face = local_faces[lf];

            if (raw_face.size() < 3)
            {
                continue;
            }

            /*
             * canonical 只用于全局面去重。
             */
            const auto face_key = canonical_face(raw_face);

            int face_id = -1;

            auto it = face_map.find(face_key);

            if (it == face_map.end())
            {
                face_id = static_cast<int>(faces_.size());

                face_map[face_key] = face_id;

                /*
                 * 这里保存 raw_face，而不是 face_key。
                 *
                 * 原因：
                 *
                 * - face_key 会排序节点，破坏面的环向顺序。
                 * - raw_face 保留 CellTopology 给出的局部面顺序。
                 * - 后续 3D 可视化和 face2edge 更可靠。
                 */
                faces_.push_back(raw_face);
                face2cell_.push_back({});

                if (lf < local_face_types.size())
                {
                    face_types_.push_back(local_face_types[lf]);
                }
                else
                {
                    face_types_.push_back(CellType::Unknown);
                }
            }
            else
            {
                face_id = it->second;
            }

            cell2face_[static_cast<std::size_t>(cell_id)].push_back(face_id);
            face2cell_[static_cast<std::size_t>(face_id)].push_back(cell_id);

            /*
             * 使用 raw_face 的环向顺序构建 face2edge。
             */
            for (std::size_t k = 0; k < raw_face.size(); ++k)
            {
                const int a = raw_face[k];
                const int b = raw_face[(k + 1) % raw_face.size()];

                const auto face_edge = canonical_edge(a, b);
                auto edge_it = edge_map.find(face_edge);

                if (edge_it != edge_map.end())
                {
                    face_edge_accumulator[face_id].insert(edge_it->second);
                }
            }

            /*
             * node2face
             *
             * 使用 raw_face 或 face_key 都可以。
             * 这里使用 raw_face，保留原始局部面节点集合。
             */
            for (int node_id : raw_face)
            {
                if (node_id >= 0 &&
                    static_cast<std::size_t>(node_id) < node_count)
                {
                    node2face_[static_cast<std::size_t>(node_id)].push_back(face_id);
                }
            }
        }
    }

    /*
     * 第二阶段：
     *
     * 根据 face_edge_accumulator 构建：
     *
     * - face2edge_
     * - edge2face_
     */
    edge2face_.resize(edges_.size());
    face2edge_.resize(faces_.size());

    for (const auto& item : face_edge_accumulator)
    {
        const int face_id = item.first;

        if (face_id < 0 ||
            static_cast<std::size_t>(face_id) >= face2edge_.size())
        {
            continue;
        }

        for (int edge_id : item.second)
        {
            if (edge_id < 0 ||
                static_cast<std::size_t>(edge_id) >= edge2face_.size())
            {
                continue;
            }

            face2edge_[static_cast<std::size_t>(face_id)].push_back(edge_id);
            edge2face_[static_cast<std::size_t>(edge_id)].push_back(face_id);
        }
    }

    /*
     * 第三阶段：
     *
     * 构建边界信息。
     *
     * 3D 体网格：
     *
     * - face2cell.size() == 1 的面是边界面
     * - 边界面上的边是边界边
     * - 边界面上的节点是边界节点
     * - 包含边界面的单元是边界单元
     *
     * 2D 面网格：
     *
     * - edge2cell.size() == 1 的边是边界边
     * - 边界边上的节点是边界节点
     * - 包含边界边的单元是边界单元
     */
    std::set<int> boundary_edge_set;
    std::set<int> boundary_face_set;
    std::set<int> boundary_node_set;
    std::set<int> boundary_cell_set;

    if (!faces_.empty())
    {
        /*
         * 3D 体网格边界。
         */
        for (std::size_t fi = 0; fi < face2cell_.size(); ++fi)
        {
            if (face2cell_[fi].size() == 1)
            {
                const int face_id = static_cast<int>(fi);
                const int cell_id = face2cell_[fi][0];

                boundary_face_set.insert(face_id);
                boundary_cell_set.insert(cell_id);

                for (int node_id : faces_[fi])
                {
                    boundary_node_set.insert(node_id);
                }

                if (fi < face2edge_.size())
                {
                    for (int edge_id : face2edge_[fi])
                    {
                        boundary_edge_set.insert(edge_id);
                    }
                }
            }
        }
    }
    else
    {
        /*
         * 2D 面网格边界。
         */
        for (std::size_t ei = 0; ei < edge2cell_.size(); ++ei)
        {
            if (edge2cell_[ei].size() == 1)
            {
                const int edge_id = static_cast<int>(ei);
                const int cell_id = edge2cell_[ei][0];

                boundary_edge_set.insert(edge_id);
                boundary_cell_set.insert(cell_id);

                boundary_node_set.insert(edges_[ei][0]);
                boundary_node_set.insert(edges_[ei][1]);
            }
        }
    }

    boundary_edges_.assign(
        boundary_edge_set.begin(),
        boundary_edge_set.end()
    );

    boundary_faces_.assign(
        boundary_face_set.begin(),
        boundary_face_set.end()
    );

    boundary_nodes_.assign(
        boundary_node_set.begin(),
        boundary_node_set.end()
    );

    boundary_cells_.assign(
        boundary_cell_set.begin(),
        boundary_cell_set.end()
    );

    /*
     * 第四阶段：
     *
     * 构建 cell2cell。
     *
     * 2D：
     *
     * - 两个单元共享一条边，则互为相邻单元。
     *
     * 3D：
     *
     * - 两个单元共享一个面，则互为相邻单元。
     */
    if (!faces_.empty())
    {
        /*
         * 3D 体网格使用 face2cell 判断邻接。
         */
        for (const auto& adjacent_cells : face2cell_)
        {
            if (adjacent_cells.size() < 2)
            {
                continue;
            }

            for (std::size_t i = 0; i < adjacent_cells.size(); ++i)
            {
                for (std::size_t j = i + 1; j < adjacent_cells.size(); ++j)
                {
                    const int c0 = adjacent_cells[i];
                    const int c1 = adjacent_cells[j];

                    if (c0 >= 0 &&
                        c1 >= 0 &&
                        static_cast<std::size_t>(c0) < cell2cell_.size() &&
                        static_cast<std::size_t>(c1) < cell2cell_.size())
                    {
                        cell2cell_[static_cast<std::size_t>(c0)].push_back(c1);
                        cell2cell_[static_cast<std::size_t>(c1)].push_back(c0);
                    }
                }
            }
        }
    }
    else
    {
        /*
         * 2D 面网格使用 edge2cell 判断邻接。
         */
        for (const auto& adjacent_cells : edge2cell_)
        {
            if (adjacent_cells.size() < 2)
            {
                continue;
            }

            for (std::size_t i = 0; i < adjacent_cells.size(); ++i)
            {
                for (std::size_t j = i + 1; j < adjacent_cells.size(); ++j)
                {
                    const int c0 = adjacent_cells[i];
                    const int c1 = adjacent_cells[j];

                    if (c0 >= 0 &&
                        c1 >= 0 &&
                        static_cast<std::size_t>(c0) < cell2cell_.size() &&
                        static_cast<std::size_t>(c1) < cell2cell_.size())
                    {
                        cell2cell_[static_cast<std::size_t>(c0)].push_back(c1);
                        cell2cell_[static_cast<std::size_t>(c1)].push_back(c0);
                    }
                }
            }
        }
    }

    /*
     * 第五阶段：
     *
     * 所有邻接表排序并去重。
     */
    for (auto& v : cell2edge_)
    {
        sort_unique(v);
    }

    for (auto& v : cell2face_)
    {
        sort_unique(v);
    }

    for (auto& v : edge2cell_)
    {
        sort_unique(v);
    }

    for (auto& v : face2cell_)
    {
        sort_unique(v);
    }

    for (auto& v : node2edge_)
    {
        sort_unique(v);
    }

    for (auto& v : node2face_)
    {
        sort_unique(v);
    }

    for (auto& v : node2cell_)
    {
        sort_unique(v);
    }

    for (auto& v : node2node_)
    {
        sort_unique(v);
    }

    for (auto& v : edge2face_)
    {
        sort_unique(v);
    }

    for (auto& v : face2edge_)
    {
        sort_unique(v);
    }

    for (auto& v : cell2cell_)
    {
        sort_unique(v);
    }

    sort_unique(boundary_edges_);
    sort_unique(boundary_faces_);
    sort_unique(boundary_nodes_);
    sort_unique(boundary_cells_);

    built_ = true;
}

} // namespace OpenCAX
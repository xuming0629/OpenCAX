#pragma once

/**
 * @file MeshTopology.h
 * @brief OpenCAX Mesh2 全局拓扑关系管理类
 *
 * MeshTopology 负责根据 Mesh 数据构建全局拓扑关系。
 *
 * Mesh 类只保存：
 *
 * - nodes
 * - cells
 *
 * 不负责保存邻接关系。
 *
 * 所有复杂拓扑均由 MeshTopology 动态构建。
 *
 *
 * ============================================================
 * 支持拓扑关系
 * ============================================================
 *
 * 一维：
 *
 * - edge2cell
 *
 *
 * 二维：
 *
 * - cell2edge
 * - edge2cell
 * - node2edge
 * - node2node
 * - node2cell
 * - cell2cell
 *
 *
 * 三维：
 *
 * - face2cell
 * - cell2face
 * - face2edge
 * - edge2face
 *
 *
 * 边界：
 *
 * - boundary_edges
 * - boundary_faces
 * - boundary_nodes
 * - boundary_cells
 *
 *
 * ============================================================
 * 高阶单元说明
 * ============================================================
 *
 * OpenCAX 支持：
 *
 * - Triangle6
 * - Quad8
 * - Quad9
 * - Tetra10
 * - Hexa20
 * - Hexa27
 *
 *
 * 当前 MeshTopology 默认采用：
 *
 * "角点拓扑"
 *
 * 即：
 *
 * Triangle6:
 *
 *     v0----m01----v1
 *
 * 在拓扑中认为：
 *
 *     v0 -------- v1
 *
 *
 * 原因：
 *
 * - FEM 自由度管理属于 FunctionSpace
 * - 高阶节点管理属于高阶元素模块
 * - MeshTopology 只负责几何邻接
 *
 *
 * 如果需要显示高阶曲线：
 *
 * 使用：
 *
 * CellTopology::visual_edges()
 *
 *
 * ============================================================
 * 使用示例
 * ============================================================
 *
 * TriangleMesh mesh;
 *
 * MeshTopology topo;
 *
 * topo.build(mesh);
 *
 * topo.node2cell();
 * topo.cell2edge();
 * topo.boundary_nodes();
 *
 */


#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/Mesh.h>

#include <array>
#include <cstddef>
#include <vector>


namespace OpenCAX
{


/**
 * @brief 拓扑构建等级
 *
 * 用于控制 MeshTopology 构建深度。
 *
 *
 * None:
 *
 * 不构建任何拓扑。
 *
 *
 * Edges:
 *
 * 构建：
 *
 * - edge
 * - cell2edge
 * - edge2cell
 * - node2edge
 * - node2node
 *
 *
 * Faces:
 *
 * 在 Edges 基础上增加：
 *
 * - face
 * - cell2face
 * - face2cell
 *
 *
 * Full:
 *
 * 完整构建：
 *
 * - 所有二维关系
 * - 所有三维关系
 * - cell2cell
 *
 */
enum class TopologyLevel
{
    None  = 0,

    /**
     * @brief 只构建边拓扑
     */
    Edges = 1,

    /**
     * @brief 构建边和面拓扑
     */
    Faces = 2,

    /**
     * @brief 完整拓扑
     */
    Full  = 3
};



/**
 * @brief OpenCAX 全局网格拓扑类
 *
 * MeshTopology 是 Mesh 的拓扑扩展。
 *
 * Mesh:
 *
 *     节点 + 单元
 *
 *
 * MeshTopology:
 *
 *     节点/边/面/单元之间关系
 *
 *
 * 典型应用：
 *
 * - FEM 装配
 * - 边界条件搜索
 * - 网格遍历
 * - 自适应加密
 * - 后处理
 * - 网格质量分析
 */
class OpenCAX_API MeshTopology
{
public:


    /**
     * @brief 清空所有拓扑数据
     */
    void clear();



    /**
     * @brief 根据网格构建拓扑
     *
     * @param mesh 输入网格
     *
     * @param level 构建等级
     *
     */
    void build(
        const Mesh& mesh,
        TopologyLevel level = TopologyLevel::Full
    );



    /**
     * @brief 判断拓扑是否已经构建
     */
    bool built() const;



    /**
     * @brief 获取当前拓扑等级
     */
    TopologyLevel level() const;



    /**
     * @brief 网格节点数量
     */
    std::size_t num_nodes() const;



    /**
     * @brief 网格单元数量
     */
    std::size_t num_cells() const;



    /**
     * @brief 边数量
     */
    std::size_t num_edges() const;



    /**
     * @brief 面数量
     */
    std::size_t num_faces() const;



    /**
     * @brief 获取所有边
     *
     * edge:
     *
     * {
     *    node0,
     *    node1
     * }
     */
    const std::vector<std::array<int,2>>& edges() const;



    /**
     * @brief 获取所有面
     */
    const std::vector<std::vector<int>>& faces() const;



    /**
     * @brief 面类型
     *
     * 例如：
     *
     * Triangle3
     * Quad4
     */
    const std::vector<CellType>& face_types() const;



    /*
     * ========================================================
     * Cell -> topology
     * ========================================================
     */


    /**
     * @brief 单元到边
     */
    const std::vector<std::vector<int>>& cell2edge() const;



    /**
     * @brief 单元到面
     */
    const std::vector<std::vector<int>>& cell2face() const;



    /**
     * @brief 边到单元
     */
    const std::vector<std::vector<int>>& edge2cell() const;



    /**
     * @brief 面到单元
     */
    const std::vector<std::vector<int>>& face2cell() const;



    /*
     * ========================================================
     * Node adjacency
     * ========================================================
     */


    /**
     * @brief 节点关联边
     */
    const std::vector<std::vector<int>>& node2edge() const;



    /**
     * @brief 节点关联面
     */
    const std::vector<std::vector<int>>& node2face() const;



    /**
     * @brief 节点关联单元
     */
    const std::vector<std::vector<int>>& node2cell() const;



    /**
     * @brief 节点邻接节点
     *
     * 图结构：
     *
     * node graph
     */
    const std::vector<std::vector<int>>& node2node() const;



    /*
     * ========================================================
     * Higher adjacency
     * ========================================================
     */


    /**
     * @brief 边关联面
     */
    const std::vector<std::vector<int>>& edge2face() const;



    /**
     * @brief 面关联边
     */
    const std::vector<std::vector<int>>& face2edge() const;



    /**
     * @brief 单元邻接单元
     *
     * FEM:
     *
     * 用于：
     *
     * - domain decomposition
     * - adaptive refinement
     */
    const std::vector<std::vector<int>>& cell2cell() const;



    /*
     * ========================================================
     * Boundary
     * ========================================================
     */


    /**
     * @brief 边界边
     */
    const std::vector<int>& boundary_edges() const;



    /**
     * @brief 边界面
     */
    const std::vector<int>& boundary_faces() const;



    /**
     * @brief 边界节点
     */
    const std::vector<int>& boundary_nodes() const;



    /**
     * @brief 边界单元
     */
    const std::vector<int>& boundary_cells() const;



private:


    /**
     * @brief 标准化边编号
     *
     * 保证：
     *
     * (3,1)
     *
     * 转换为：
     *
     * (1,3)
     */
    static std::array<int,2> canonical_edge(
        int a,
        int b
    );



    /**
     * @brief 标准化面编号
     *
     * 用于 Hash / map 查找。
     */
    static std::vector<int> canonical_face(
        const std::vector<int>& node_ids
    );



    /**
     * @brief 排序并删除重复元素
     */
    static void sort_unique(
        std::vector<int>& values
    );



private:


    /**
     * @brief 是否已经构建
     */
    bool built_ = false;



    /**
     * @brief 当前拓扑等级
     */
    TopologyLevel level_ =
        TopologyLevel::None;



    /*
     * ========================================================
     * Basic topology
     * ========================================================
     */


    std::vector<std::array<int,2>> edges_;


    std::vector<std::vector<int>> faces_;


    std::vector<CellType> face_types_;



    /*
     * ========================================================
     * Cell adjacency
     * ========================================================
     */


    std::vector<std::vector<int>> cell2edge_;

    std::vector<std::vector<int>> cell2face_;

    std::vector<std::vector<int>> edge2cell_;

    std::vector<std::vector<int>> face2cell_;



    /*
     * ========================================================
     * Node adjacency
     * ========================================================
     */


    std::vector<std::vector<int>> node2edge_;

    std::vector<std::vector<int>> node2face_;

    std::vector<std::vector<int>> node2cell_;

    std::vector<std::vector<int>> node2node_;



    /*
     * ========================================================
     * Higher adjacency
     * ========================================================
     */


    std::vector<std::vector<int>> edge2face_;

    std::vector<std::vector<int>> face2edge_;

    std::vector<std::vector<int>> cell2cell_;



    /*
     * ========================================================
     * Boundary information
     * ========================================================
     */


    std::vector<int> boundary_edges_;

    std::vector<int> boundary_faces_;

    std::vector<int> boundary_nodes_;

    std::vector<int> boundary_cells_;

};


} // namespace OpenCAX
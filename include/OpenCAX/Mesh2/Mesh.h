#pragma once

/**
 * @file Mesh.h
 * @brief OpenCAX Mesh2 通用网格基础容器
 *
 * Mesh 是 OpenCAX 网格核心数据结构。
 *
 * 负责：
 *
 * - 保存节点 MeshNode
 * - 保存单元 MeshCell
 * - 保存 MeshInfo
 *
 * 不负责：
 *
 * - edge2node
 * - cell2edge
 * - cell2cell
 * - boundary
 *
 * 这些由 MeshTopology 完成。
 *
 * Mesh 是所有网格类型的基类：
 *
 * - TriangleMesh
 * - QuadMesh
 * - TetraMesh
 * - HexMesh
 *
 */


#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/MeshTypes.h>

#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>


namespace OpenCAX
{


/**
 * @brief 通用网格基础类
 */
class OpenCAX_API Mesh
{

public:

    virtual ~Mesh() = default;


public:

    /**
     * @brief 清空所有网格数据
     */
    void clear();



    // =====================================================
    // Mesh 信息
    // =====================================================


    /**
     * @brief 获取网格信息
     */
    MeshInfo& info();


    /**
     * @brief 获取只读网格信息
     */
    const MeshInfo& info() const;



    /**
     * @brief 设置网格名称
     */
    void set_name(
        const std::string& name
    );


    /**
     * @brief 设置网格来源
     *
     * 例如：
     *
     * - Structured
     * - Gmsh
     * - TetGen
     */
    void set_source(
        const std::string& source
    );


    /**
     * @brief 设置网格维度
     */
    void set_dimension(
        MeshDimension dim
    );


    /**
     * @brief 设置网格来源类型
     */
    void set_source_type(
        MeshSourceType type
    );



    // =====================================================
    // 节点管理
    // =====================================================


    /**
     * @brief 获取节点数组
     */
    std::vector<MeshNode>& nodes();


    /**
     * @brief 获取只读节点数组
     */
    const std::vector<MeshNode>& nodes() const;



    /**
     * @brief 获取节点
     *
     * @param id 节点编号
     */
    MeshNode& node(
        int id
    );


    /**
     * @brief 获取只读节点
     */
    const MeshNode& node(
        int id
    ) const;



    /**
     * @brief 添加节点
     *
     * @return 新节点 ID
     */
    int add_node(
        double x,
        double y,
        double z = 0.0
    );


    /**
     * @brief 添加带属性节点
     *
     * 用于：
     *
     * - Gmsh Physical Group
     * - CAD Vertex
     * - Boundary
     */
    int add_node(
        double x,
        double y,
        double z,
        int physical_id,
        int boundary_id = -1,
        int geometry_id = -1
    );



    // =====================================================
    // 单元管理
    // =====================================================


    /**
     * @brief 获取单元数组
     */
    std::vector<MeshCell>& cells();


    /**
     * @brief 获取只读单元数组
     */
    const std::vector<MeshCell>& cells() const;



    /**
     * @brief 获取单元
     */
    MeshCell& cell(
        int id
    );


    /**
     * @brief 获取只读单元
     */
    const MeshCell& cell(
        int id
    ) const;



    /**
     * @brief 添加单元
     */
    int add_cell(
        CellType type,
        const std::vector<int>& node_ids
    );



    /**
     * @brief 添加带属性单元
     *
     * 支持：
     *
     * - material_id
     * - region_id
     * - physical_id
     */
    int add_cell(
        CellType type,
        const std::vector<int>& node_ids,
        int physical_id,
        int material_id = -1,
        int region_id = -1,
        int geometry_id = -1
    );



    // =====================================================
    // 查询
    // =====================================================


    /**
     * @brief 判断节点编号是否合法
     */
    bool valid_node_id(
        int node_id
    ) const;


    /**
     * @brief 判断单元编号是否合法
     */
    bool valid_cell_id(
        int cell_id
    ) const;



    /**
     * @brief 节点数量
     */
    std::size_t num_nodes() const;


    /**
     * @brief 单元数量
     */
    std::size_t num_cells() const;



    /**
     * @brief 是否为空
     */
    bool empty() const;



    /**
     * @brief 获取网格包围盒
     *
     * 返回：
     *
     * {
     * xmin,
     * xmax,
     * ymin,
     * ymax,
     * zmin,
     * zmax
     * }
     */
    std::array<double,6> bounds() const;



    /**
     * @brief 网格基础合法性检查
     *
     * 检查：
     *
     * - node id 连续
     * - cell id 连续
     * - node_ids 有效
     * - CellType 合法
     */
    virtual bool validate(
        std::string* error_message = nullptr
    ) const;



    // =====================================================
    // 高级接口
    // =====================================================


    /**
     * @brief 深复制网格
     *
     * 用于：
     *
     * - refine
     * - high order conversion
     * - optimization
     */
    virtual std::shared_ptr<Mesh> clone() const;



    /**
     * @brief 获取网格最高阶次
     *
     * 例如：
     *
     * Triangle3:
     *
     * return 1
     *
     * Triangle6:
     *
     * return 2
     */
    int order() const;



    /**
     * @brief 判断是否高阶网格
     */
    bool is_high_order() const;



    /**
     * @brief 获取单元类型集合
     *
     * 返回：
     *
     * Triangle3
     * Triangle6
     * Quad4
     */
    std::vector<CellType> cell_types() const;



    /**
     * @brief 获取节点维度
     *
     * 对 MeshDimension 的辅助判断
     */
    MeshDimension dimension() const;



protected:

    /**
     * @brief 网格信息
     */
    MeshInfo info_;


    /**
     * @brief 节点数组
     */
    std::vector<MeshNode> nodes_;


    /**
     * @brief 单元数组
     */
    std::vector<MeshCell> cells_;

};



} // namespace OpenCAX
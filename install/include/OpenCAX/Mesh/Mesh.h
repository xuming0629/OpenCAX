#pragma once

/**
 * @file Mesh.h
 * @brief OpenCAX 通用网格基础容器声明
 *
 * Mesh 是 OpenCAX 网格模块的基础数据结构。
 *
 * 它负责保存：
 *
 * - 网格基础信息 MeshInfo
 * - 网格节点 MeshNode
 * - 网格单元 MeshCell
 *
 * Mesh 本身只负责基础数据存储，不负责复杂拓扑关系。
 *
 * 复杂拓扑关系，例如：
 *
 * - edge2node
 * - face2node 
 * - cell2edge
 * - cell2face
 * - edge2cell
 * - face2cell
 * - node2cell
 * - node2node
 * - cell2cell
 *
 * 应由 MeshTopology 构建。
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/MeshTypes.h>

#include <cstddef>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 通用网格基础容器
 *
 * Mesh 是 OpenCAX 网格模块的基础容器类。
 *
 * 它统一保存节点、单元和网格元信息。
 *
 * 设计原则：
 *
 * - Mesh 只负责数据存储
 * - MeshTopology 负责拓扑关系构建
 * - TriangleMesh / QuadMesh / TetraMesh / HexMesh 等派生类负责特定网格的几何计算
 *
 * 支持派生：
 *
 * - TriangleMesh
 * - QuadMesh
 * - TetraMesh
 * - HexMesh
 *
 * @note
 * Mesh 中节点 ID 和单元 ID 默认从 0 开始连续编号。
 */
class OpenCAX_API Mesh
{
public:
    /**
     * @brief 虚析构函数
     *
     * 支持通过基类指针安全释放派生类对象。
     */
    virtual ~Mesh() = default;

    /**
     * @brief 清空网格
     *
     * 清空网格元信息、节点数组和单元数组。
     */
    void clear();

    /**
     * @brief 获取网格信息
     *
     * @return 可修改的 MeshInfo 引用
     */
    MeshInfo& info();

    /**
     * @brief 获取网格信息
     *
     * @return 只读 MeshInfo 引用
     */
    const MeshInfo& info() const;

    /**
     * @brief 获取节点数组
     *
     * @return 可修改的节点数组引用
     */
    std::vector<MeshNode>& nodes();

    /**
     * @brief 获取节点数组
     *
     * @return 只读节点数组引用
     */
    const std::vector<MeshNode>& nodes() const;

    /**
     * @brief 获取单元数组
     *
     * @return 可修改的单元数组引用
     */
    std::vector<MeshCell>& cells();

    /**
     * @brief 获取单元数组
     *
     * @return 只读单元数组引用
     */
    const std::vector<MeshCell>& cells() const;

    /**
     * @brief 添加节点
     *
     * @param x X 坐标
     * @param y Y 坐标
     * @param z Z 坐标，二维平面网格通常为 0
     * @return 新增节点 ID
     */
    int add_node(
        double x,
        double y,
        double z = 0.0
    );

    /**
     * @brief 添加节点
     *
     * 可同时指定物理分组 ID 和边界 ID。
     *
     * @param x X 坐标
     * @param y Y 坐标
     * @param z Z 坐标
     * @param physical_id 物理分组 ID
     * @param boundary_id 边界 ID
     * @return 新增节点 ID
     */
    int add_node(
        double x,
        double y,
        double z,
        int physical_id,
        int boundary_id = -1
    );

    /**
     * @brief 添加单元
     *
     * @param type 单元类型
     * @param node_ids 单元节点 ID 列表
     * @return 新增单元 ID
     */
    int add_cell(
        CellType type,
        const std::vector<int>& node_ids
    );

    /**
     * @brief 添加单元
     *
     * 可同时指定物理分组、材料和区域 ID。
     *
     * @param type 单元类型
     * @param node_ids 单元节点 ID 列表
     * @param physical_id 物理分组 ID
     * @param material_id 材料 ID
     * @param region_id 区域 ID
     * @return 新增单元 ID
     */
    int add_cell(
        CellType type,
        const std::vector<int>& node_ids,
        int physical_id,
        int material_id = -1,
        int region_id = -1
    );

    /**
     * @brief 判断节点 ID 是否有效
     *
     * @param node_id 节点 ID
     * @return 如果节点 ID 有效，返回 true；否则返回 false
     */
    bool valid_node_id(
        int node_id
    ) const;

    /**
     * @brief 判断单元 ID 是否有效
     *
     * @param cell_id 单元 ID
     * @return 如果单元 ID 有效，返回 true；否则返回 false
     */
    bool valid_cell_id(
        int cell_id
    ) const;

    /**
     * @brief 获取节点数量
     *
     * @return 节点数量
     */
    std::size_t num_nodes() const;

    /**
     * @brief 获取单元数量
     *
     * @return 单元数量
     */
    std::size_t num_cells() const;

    /**
     * @brief 判断网格是否为空
     *
     * @return 如果没有节点且没有单元，返回 true；否则返回 false
     */
    bool empty() const;

    /**
     * @brief 检查网格基础有效性
     *
     * 检查内容：
     *
     * - 所有单元节点 ID 是否有效
     * - 所有单元类型是否不是 Unknown
     * - 所有节点和单元 ID 是否与数组下标一致
     *
     * @return 如果网格基础数据有效，返回 true；否则返回 false
     */
    bool validate() const;

protected:
    /**
     * @brief 网格元信息
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
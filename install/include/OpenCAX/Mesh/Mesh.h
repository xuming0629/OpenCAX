#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/MeshTypes.h>

#include <cstddef>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 通用网格基础容器
 *
 * Mesh 是 OpenCAX 网格模块的基础数据结构，
 * 用于保存网格节点、网格单元以及网格元信息。
 *
 * Mesh 本身只负责基础数据存储，不负责复杂拓扑关系。
 *
 * 复杂拓扑关系，例如：
 *
 * - edge2node
 * - cell2edge
 * - edge2cell
 * - node2cell
 * - node2node
 * - cell2cell
 * - face2cell
 *
 * 应由 MeshTopology 构建。
 */
class OpenCAX_API Mesh
{
public:
    /**
     * @brief 虚析构函数
     *
     * 支持 TriangleMesh、QuadMesh、TetraMesh、HexMesh 等派生类。
     */
    virtual ~Mesh() = default;

    /**
     * @brief 清空网格
     *
     * 清空网格信息、节点和单元。
     */
    void clear();

    /**
     * @brief 获取网格信息（可修改）
     */
    MeshInfo& info();

    /**
     * @brief 获取网格信息（只读）
     */
    const MeshInfo& info() const;

    /**
     * @brief 获取节点数组（可修改）
     */
    std::vector<MeshNode>& nodes();

    /**
     * @brief 获取单元数组（可修改）
     */
    std::vector<MeshCell>& cells();

    /**
     * @brief 获取节点数组（只读）
     */
    const std::vector<MeshNode>& nodes() const;

    /**
     * @brief 获取单元数组（只读）
     */
    const std::vector<MeshCell>& cells() const;

    /**
     * @brief 添加节点
     *
     * @param x X 坐标
     * @param y Y 坐标
     * @param z Z 坐标，二维网格默认 0
     * @return 新节点 ID
     */
    int add_node(
        double x,
        double y,
        double z = 0.0
    );

    /**
     * @brief 添加单元
     *
     * @param type 单元类型
     * @param node_ids 单元节点 ID 列表
     * @return 新单元 ID
     */
    int add_cell(
        CellType type,
        const std::vector<int>& node_ids
    );

    /**
     * @brief 添加单元
     *
     * 可同时指定物理分组、材料和区域 ID。
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
     */
    bool valid_node_id(
        int node_id
    ) const;

    /**
     * @brief 判断单元 ID 是否有效
     */
    bool valid_cell_id(
        int cell_id
    ) const;

    /**
     * @brief 获取节点数量
     */
    std::size_t num_nodes() const;

    /**
     * @brief 获取单元数量
     */
    std::size_t num_cells() const;

    /**
     * @brief 判断网格是否为空
     */
    bool empty() const;

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
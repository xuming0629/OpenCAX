#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 四面体网格
 *
 * TetraMesh 用于表示三维四面体体网格。
 *
 * 设计说明：
 *
 * - 继承自 Mesh
 * - 节点使用 MeshNode{x, y, z}
 * - 单元类型固定为 CellType::Tetra4
 *
 * 支持：
 *
 * - 手动添加节点和四面体
 * - 非结构四面体网格构造
 * - 结构化六面体区域四面体剖分
 * - 四面体体积计算
 * - 四面体重心计算
 * - 边提取
 * - 面提取
 * - 边界面提取
 * - 边界节点提取
 *
 * 后续可直接对接：
 *
 * - TetGen
 * - Gmsh 3D 四面体网格
 * - FEM Tetra4 / Tetra10 单元
 * - VTK 可视化
 */
class OpenCAX_API TetraMesh : public Mesh
{
public:

    /**
     * @brief 添加三维节点
     *
     * @param x X 坐标
     * @param y Y 坐标
     * @param z Z 坐标
     * @return 新节点 ID
     */
    int add_node_3d(
        double x,
        double y,
        double z
    );

    /**
     * @brief 添加四面体单元
     *
     * @param n0 第 0 个节点 ID
     * @param n1 第 1 个节点 ID
     * @param n2 第 2 个节点 ID
     * @param n3 第 3 个节点 ID
     * @return 新单元 ID
     */
    int add_tetra(
        int n0,
        int n1,
        int n2,
        int n3
    );

    /**
     * @brief 添加四面体单元，并指定分组信息
     *
     * @param n0 第 0 个节点 ID
     * @param n1 第 1 个节点 ID
     * @param n2 第 2 个节点 ID
     * @param n3 第 3 个节点 ID
     * @param physical_id 物理分组 ID
     * @param material_id 材料 ID
     * @param region_id 区域 ID
     * @return 新单元 ID
     */
    int add_tetra(
        int n0,
        int n1,
        int n2,
        int n3,
        int physical_id,
        int material_id = -1,
        int region_id = -1
    );

    /**
     * @brief 获取四面体数量
     */
    std::size_t num_tetras() const;

    /**
     * @brief 获取四面体节点 ID
     *
     * @param cell_id 单元 ID
     * @return 四个节点 ID
     */
    std::array<int, 4> tetra(
        std::size_t cell_id
    ) const;

    /**
     * @brief 获取三维节点坐标
     *
     * @param node_id 节点 ID
     * @return {x, y, z}
     */
    std::array<double, 3> point3d(
        int node_id
    ) const;

    /**
     * @brief 四面体有向体积
     *
     * 若节点顺序为正向，返回正值；
     * 若节点顺序反向，返回负值。
     *
     * @param cell_id 单元 ID
     * @return 有向体积
     */
    double signed_tetra_volume(
        std::size_t cell_id
    ) const;

    /**
     * @brief 四面体体积
     *
     * @param cell_id 单元 ID
     * @return 体积
     */
    double tetra_volume(
        std::size_t cell_id
    ) const;

    /**
     * @brief 总体积
     *
     * @return 所有四面体体积之和
     */
    double total_volume() const;

    /**
     * @brief 四面体重心
     *
     * @param cell_id 单元 ID
     * @return {cx, cy, cz}
     */
    std::array<double, 3> tetra_centroid(
        std::size_t cell_id
    ) const;

    /**
     * @brief 判断是否所有单元都是 Tetra4
     */
    bool is_pure_tetra_mesh() const;

    /**
     * @brief 提取所有唯一边
     *
     * 每条边标准化为 {min_id, max_id}。
     */
    std::vector<std::array<int, 2>> edges() const;

    /**
     * @brief 提取所有唯一面
     *
     * 每个面标准化为排序后的三个节点 ID。
     */
    std::vector<std::array<int, 3>> faces() const;

    /**
     * @brief 提取边界面
     *
     * 只被一个四面体使用的面是边界面。
     */
    std::vector<std::array<int, 3>> boundary_faces() const;

    /**
     * @brief 提取边界节点
     */
    std::vector<int> boundary_nodes() const;

    /**
     * @brief 创建非结构四面体网格
     *
     * @param points 三维节点坐标
     * @param tetras 四面体连接关系
     */
    static TetraMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 4>>& tetras
    );

    /**
     * @brief 创建结构化六面体区域四面体网格
     *
     * 将长方体区域 [xmin,xmax] x [ymin,ymax] x [zmin,zmax]
     * 划分为 nx * ny * nz 个小六面体，
     * 每个小六面体剖分为 6 个四面体。
     */
    static TetraMesh create_structured_box(
        double xmin,
        double xmax,
        double ymin,
        double ymax,
        double zmin,
        double zmax,
        int nx,
        int ny,
        int nz
    );

private:

    /**
     * @brief 标准化边
     */
    static std::array<int, 2> make_edge(
        int a,
        int b
    );

    /**
     * @brief 标准化三角面
     */
    static std::array<int, 3> make_face(
        int a,
        int b,
        int c
    );

    /**
     * @brief 检查节点 ID
     */
    void check_node_id(
        int node_id
    ) const;

    /**
     * @brief 检查四面体节点合法性
     */
    void check_tetra_nodes(
        int n0,
        int n1,
        int n2,
        int n3
    ) const;
};

} // namespace OpenCAX
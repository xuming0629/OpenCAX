#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 二维三角形网格
 *
 * TriangleMesh 是 OpenCAX 中用于表示二维三角形网格的专用网格类。
 *
 * 设计说明：
 *
 * - 继承自 Mesh
 * - 节点仍然使用 MeshNode{x, y, z}
 * - 对于二维平面网格，z = 0.0
 * - 单元类型固定为 CellType::Triangle3
 *
 * 支持：
 *
 * - 手动添加节点与三角形
 * - 结构化矩形三角剖分
 * - 非结构三角形网格
 * - 三角形面积计算
 * - 三角形重心计算
 * - 边提取
 * - 边界边提取
 * - 边界节点提取
 *
 * 后续可直接对接：
 *
 * - Gmsh 2D 三角网格
 * - FEM P1/P2 三角形单元
 * - VTK 可视化
 */
class OpenCAX_API TriangleMesh : public Mesh
{
public:

    /**
     * @brief 添加二维节点
     *
     * @param x X 坐标
     * @param y Y 坐标
     * @return 新节点 ID
     */
    int add_node_2d(
        double x,
        double y
    );

    /**
     * @brief 添加三角形单元
     *
     * @param n0 第 0 个节点 ID
     * @param n1 第 1 个节点 ID
     * @param n2 第 2 个节点 ID
     * @return 新单元 ID
     */
    int add_triangle(
        int n0,
        int n1,
        int n2
    );

    /**
     * @brief 添加三角形单元，并指定分组信息
     *
     * @param n0 第 0 个节点 ID
     * @param n1 第 1 个节点 ID
     * @param n2 第 2 个节点 ID
     * @param physical_id 物理分组 ID
     * @param material_id 材料 ID
     * @param region_id 区域 ID
     * @return 新单元 ID
     */
    int add_triangle(
        int n0,
        int n1,
        int n2,
        int physical_id,
        int material_id = -1,
        int region_id = -1
    );

    /**
     * @brief 获取三角形数量
     */
    std::size_t num_triangles() const;

    /**
     * @brief 获取三角形单元节点 ID
     *
     * @param cell_id 单元 ID
     * @return 三个节点 ID
     */
    std::array<int, 3> triangle(
        std::size_t cell_id
    ) const;

    /**
     * @brief 获取二维节点坐标
     *
     * @param node_id 节点 ID
     * @return {x, y}
     */
    std::array<double, 2> point2d(
        int node_id
    ) const;

    /**
     * @brief 计算三角形有向面积
     *
     * 若节点为逆时针顺序，返回正值；
     * 若节点为顺时针顺序，返回负值。
     *
     * @param cell_id 单元 ID
     * @return 有向面积
     */
    double signed_triangle_area(
        std::size_t cell_id
    ) const;

    /**
     * @brief 计算三角形面积
     *
     * @param cell_id 单元 ID
     * @return 三角形面积
     */
    double triangle_area(
        std::size_t cell_id
    ) const;

    /**
     * @brief 计算总面积
     *
     * @return 所有三角形面积之和
     */
    double total_area() const;

    /**
     * @brief 计算三角形重心
     *
     * @param cell_id 单元 ID
     * @return {cx, cy}
     */
    std::array<double, 2> triangle_centroid(
        std::size_t cell_id
    ) const;

    /**
     * @brief 判断是否所有单元都是 Triangle3
     */
    bool is_pure_triangle_mesh() const;

    /**
     * @brief 获取所有唯一边
     *
     * 边会被标准化为 {min_id, max_id}。
     */
    std::vector<std::array<int, 2>> edges() const;

    /**
     * @brief 获取边界边
     *
     * 只被一个三角形使用的边是边界边。
     */
    std::vector<std::array<int, 2>> boundary_edges() const;

    /**
     * @brief 获取边界节点
     */
    std::vector<int> boundary_nodes() const;

    /**
     * @brief 创建结构化矩形三角形网格
     *
     * 将矩形区域 [xmin, xmax] x [ymin, ymax]
     * 划分为 nx * ny 个矩形，
     * 每个矩形再切分为两个三角形。
     */
    static TriangleMesh create_structured_rectangle(
        double xmin,
        double xmax,
        double ymin,
        double ymax,
        int nx,
        int ny
    );

    /**
     * @brief 创建非结构三角形网格
     *
     * @param points 二维节点坐标
     * @param triangles 三角形连接关系
     */
    static TriangleMesh create_unstructured(
        const std::vector<std::array<double, 2>>& points,
        const std::vector<std::array<int, 3>>& triangles
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
     * @brief 检查节点 ID
     */
    void check_node_id(
        int node_id
    ) const;

    /**
     * @brief 检查三角形节点合法性
     */
    void check_triangle_nodes(
        int n0,
        int n1,
        int n2
    ) const;
};

} // namespace OpenCAX
#pragma once

/**
 * @file TriangleMesh.h
 * @brief OpenCAX Mesh2 二维三角形网格类声明
 *
 * TriangleMesh 是 Mesh 的派生类，专门用于管理二维三角形网格。
 *
 * 当前支持：
 *
 * - 结构三角形网格
 * - 非结构三角形网格
 * - Triangle3 一阶三角形
 * - Triangle6 二阶三角形的基础面积/质心计算
 *
 * @note
 * TriangleMesh 只负责三角形网格本身的数据管理和几何计算。
 * 全局拓扑关系，例如：
 *
 * - edge
 * - cell2edge
 * - edge2cell
 * - node2cell
 * - cell2cell
 * - boundary_edges
 *
 * 应由 MeshTopology 构建。
 *
 * @note
 * create_structured_rectangle() 生成的是 Triangle3 网格。
 * 若需要 Triangle6，可通过 Meshing/HighOrder 中的转换器完成：
 *
 * @code
 * auto tri3 = TriangleMesh::create_structured_rectangle(...);
 * auto tri6 = MeshOrderConverter::triangle3_to_triangle6(tri3);
 * @endcode
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/Mesh.h>

#include <array>
#include <string>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 二维三角形网格类
 *
 * TriangleMesh 继承自 Mesh。
 *
 * 其单元类型主要为：
 *
 * - CellType::Triangle3
 * - CellType::Triangle6
 *
 * 常见用途：
 *
 * - 二维有限元网格
 * - 面网格离散
 * - Poisson2D / Heat2D / Elasticity2D
 * - 三角网格加密
 * - 三角网格升阶
 */
class OpenCAX_API TriangleMesh : public Mesh
{
public:
    /**
     * @brief 构造空三角形网格
     *
     * 默认初始化 MeshInfo：
     *
     * - name       = "TriangleMesh"
     * - dimension  = MeshDimension::Dim2
     * - source     = "OpenCAX"
     */
    TriangleMesh();

    /**
     * @brief 创建矩形区域上的结构三角形网格
     *
     * 将矩形区域：
     *
     * @code
     * [xmin, xmax] × [ymin, ymax]
     * @endcode
     *
     * 划分为 nx × ny 个四边形小格子。
     *
     * 每个小四边形再拆分为 2 个三角形。
     *
     * @param xmin X 方向最小坐标
     * @param xmax X 方向最大坐标
     * @param ymin Y 方向最小坐标
     * @param ymax Y 方向最大坐标
     * @param nx X 方向划分数量，必须大于 0
     * @param ny Y 方向划分数量，必须大于 0
     * @param alternate_diagonal 是否交替使用对角线
     *
     * @return 结构三角形网格
     *
     * @note
     * 若 alternate_diagonal=false，每个矩形使用统一方向对角线。
     *
     * @note
     * 若 alternate_diagonal=true，相邻矩形交替使用不同方向对角线，
     * 可在某些情况下改善网格方向一致性带来的数值偏差。
     */
    static TriangleMesh create_structured_rectangle(
        double xmin,
        double xmax,
        double ymin,
        double ymax,
        int nx,
        int ny,
        bool alternate_diagonal = false
    );

    /**
     * @brief 创建非结构三角形网格
     *
     * 根据外部给定节点坐标和三角形连接关系创建 Triangle3 网格。
     *
     * @param points 节点坐标数组，每个点为 {x,y,z}
     * @param triangles 三角形单元节点编号，每个三角形为 {n0,n1,n2}
     *
     * @return 非结构三角形网格
     *
     * @note
     * triangles 中的节点编号必须是 points 的合法下标。
     */
    static TriangleMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 3>>& triangles
    );

    /**
     * @brief 设置结构网格参数信息
     *
     * 该函数只保存结构网格元信息，不重新生成节点和单元。
     *
     * @param info 二维结构网格参数
     */
    void set_structured_info(
        const StructuredGridInfo2D& info
    );

    /**
     * @brief 获取结构网格参数信息
     *
     * @return 二维结构网格参数
     */
    const StructuredGridInfo2D& structured_info() const;

    /**
     * @brief 判断是否为结构三角形网格
     *
     * @return 若 MeshInfo::structured=true 返回 true
     */
    bool is_structured() const;

    /**
     * @brief 判断是否为非结构三角形网格
     *
     * @return 若不是结构网格返回 true
     */
    bool is_unstructured() const;

    /**
     * @brief 计算三角形面积
     *
     * 使用三个角点计算面积。
     *
     * 面积公式：
     *
     * @code
     * area = 0.5 * | (b-a) x (c-a) |
     * @endcode
     *
     * @param a 第一个节点
     * @param b 第二个节点
     * @param c 第三个节点
     *
     * @return 三角形面积
     *
     * @note
     * 该函数支持三维空间中的三角形曲面面积计算。
     */
    static double triangle_area(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

    /**
     * @brief 计算指定单元面积
     *
     * @param cell_id 单元编号
     *
     * @return 单元面积
     *
     * @note
     * 对 Triangle6，默认使用前三个角点节点计算面积。
     */
    double area(
        int cell_id
    ) const;

    /**
     * @brief 计算整个三角形网格总面积
     *
     * @return 所有三角形单元面积之和
     */
    double total_area() const;

    /**
     * @brief 计算指定三角形单元质心
     *
     * @param cell_id 单元编号
     *
     * @return 质心坐标 {x,y,z}
     *
     * @note
     * 对 Triangle6，默认使用前三个角点节点计算几何质心。
     */
    std::array<double, 3> centroid(
        int cell_id
    ) const;

    /**
     * @brief 三角形网格合法性检查
     *
     * 在 Mesh::validate 基础上额外检查：
     *
     * - 单元必须为 Triangle3 或 Triangle6
     * - 单元面积必须大于 area_eps
     * - 节点编号必须有效
     *
     * @param error_message 错误信息输出，可为空
     * @param area_eps 面积容差，小于等于该值认为是退化三角形
     *
     * @return 合法返回 true，否则返回 false
     */
    bool validate(
        std::string* error_message,
        double area_eps
    ) const;

    /**
     * @brief 三角形网格默认合法性检查
     *
     * 默认面积容差通常在 cpp 中设置，例如：
     *
     * @code
     * area_eps = 1.0e-14
     * @endcode
     *
     * @param error_message 错误信息输出，可为空
     *
     * @return 合法返回 true，否则返回 false
     */
    bool validate(
        std::string* error_message = nullptr
    ) const override;

private:
    /**
     * @brief 结构三角形网格参数
     *
     * 仅当 is_structured()==true 时有实际意义。
     */
    StructuredGridInfo2D structured_info_;
};

} // namespace OpenCAX
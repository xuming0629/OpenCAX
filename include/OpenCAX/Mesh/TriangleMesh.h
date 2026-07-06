#pragma once

/**
 * @file TriangleMesh.h
 * @brief OpenCAX 二维三角形网格类声明
 *
 * 本文件声明 TriangleMesh 类。
 *
 * TriangleMesh 是 Mesh 的派生类，专门用于管理由 Triangle3 单元组成的二维三角形网格。
 *
 * 当前支持两类网格来源：
 *
 * - 结构三角形网格：由规则矩形区域按照 nx × ny 划分得到
 * - 非结构三角形网格：由外部节点数组和三角形连接关系直接构造得到
 *
 * @note
 * TriangleMesh 本身只负责三角形网格的几何计算和构造。
 * 更复杂的拓扑关系，例如 boundary_edges、cell2cell、node2cell，
 * 应由 MeshTopology 构建。
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 三角形网格来源类型
 */
enum class TriangleMeshSourceType
{
    /**
     * @brief 未知来源
     */
    Unknown = 0,

    /**
     * @brief 结构网格
     *
     * 通常由规则矩形区域按照 nx × ny 划分得到。
     */
    Structured,

    /**
     * @brief 非结构网格
     *
     * 通常由外部网格生成器、导入文件或用户自定义节点连接关系得到。
     */
    Unstructured
};

/**
 * @brief 网格生成来源信息（扩展版）
 *
 * 用于记录 mesh 是如何被创建的，而不仅仅是 structured/unstructured
 */
struct TriangleMeshGeneratorInfo
{
    /**
     * @brief 生成器类型
     */
    std::string generator = "Unknown";  
    // 例如: "Structured", "Gmsh", "TetGen", "OBJ", "STL", "User"

    /**
     * @brief 来源文件（如果有）
     */
    std::string source_file;

    /**
     * @brief 额外描述信息
     */
    std::string description;

    /**
     * @brief 是否来自外部文件
     */
    bool from_file = false;
};

/**
 * @brief 结构三角形网格元信息
 *
 * 用于记录结构化矩形三角网格的生成参数。
 */
struct TriangleMeshStructuredInfo
{
    /**
     * @brief X 方向单元划分数量
     */
    int nx = 0;

    /**
     * @brief Y 方向单元划分数量
     */
    int ny = 0;

    /**
     * @brief X 最小值
     */
    double xmin = 0.0;

    /**
     * @brief X 最大值
     */
    double xmax = 0.0;

    /**
     * @brief Y 最小值
     */
    double ymin = 0.0;

    /**
     * @brief Y 最大值
     */
    double ymax = 0.0;

    /**
     * @brief 是否采用交错对角线剖分
     */
    bool alternate_diagonal = false;
};

/**
 * @brief 二维三角形网格类
 *
 * TriangleMesh 继承自 Mesh，用于保存和处理 Triangle3 网格。
 *
 * 支持：
 *
 * - 结构矩形区域三角网格生成
 * - 非结构三角网格构造
 * - 单元面积计算
 * - 总面积计算
 * - 单元质心计算
 * - 包围盒计算
 * - 三角形网格合法性检查
 */
class OpenCAX_API TriangleMesh : public Mesh
{
public:
    /**
     * @brief 默认构造函数
     */
    TriangleMesh();

    /**
     * @brief 创建结构矩形三角形网格
     *
     * 将矩形区域 [xmin, xmax] × [ymin, ymax] 划分为 nx × ny 个矩形，
     * 每个矩形再剖分为两个 Triangle3 单元。
     *
     * @param xmin X 最小值
     * @param xmax X 最大值
     * @param ymin Y 最小值
     * @param ymax Y 最大值
     * @param nx X 方向矩形划分数量，必须大于 0
     * @param ny Y 方向矩形划分数量，必须大于 0
     * @param alternate_diagonal 是否采用交错对角线剖分
     * @return 生成的结构三角形网格
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
     * @param points 节点坐标数组，每个节点为 {x, y, z}
     * @param triangles 三角形连接关系数组，每个三角形为 3 个节点 ID
     * @return 生成的非结构三角形网格
     */
    static TriangleMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 3>>& triangles
    );

    /**
     * @brief 获取网格来源类型
     *
     * @return 三角形网格来源类型
     */
    TriangleMeshSourceType source_type() const;

    /**
     * @brief 设置网格来源类型
     *
     * @param type 来源类型
     */
    void set_source_type(
        TriangleMeshSourceType type
    );

    /**
     * @brief 判断是否为结构三角形网格
     *
     * @return 如果为结构网格，返回 true；否则返回 false
     */
    bool is_structured() const;

    /**
     * @brief 判断是否为非结构三角形网格
     *
     * @return 如果为非结构网格，返回 true；否则返回 false
     */
    bool is_unstructured() const;

    /**
     * @brief 获取结构网格生成信息
     *
     * @return 结构网格生成信息
     */
    const TriangleMeshStructuredInfo& structured_info() const;

    /**
     * @brief 获取指定三角形单元面积
     *
     * @param cell_id 单元 ID
     * @return 三角形面积
     */
    double area(
        int cell_id
    ) const;

    /**
     * @brief 获取所有三角形单元总面积
     *
     * @return 网格总面积
     */
    double total_area() const;

    /**
     * @brief 获取指定三角形单元质心
     *
     * @param cell_id 单元 ID
     * @return 单元质心坐标 {x, y, z}
     */
    std::array<double, 3> centroid(
        int cell_id
    ) const;

    /**
     * @brief 获取网格包围盒
     *
     * 返回顺序为：
     *
     * - xmin
     * - xmax
     * - ymin
     * - ymax
     * - zmin
     * - zmax
     *
     * @return 包围盒数组
     */
    std::array<double, 6> bounds() const;

    /**
     * @brief 检查三角形网格是否合法
     *
     * 检查内容包括：
     *
     * - 单元类型必须为 Triangle3
     * - 每个单元必须包含 3 个节点
     * - 节点 ID 必须有效
     * - 三角形面积必须大于给定阈值
     *
     * @param error_message 错误信息输出指针，可以为空
     * @param area_eps 面积阈值
     * @return 如果网格合法，返回 true；否则返回 false
     */
    bool validate(
        std::string* error_message = nullptr,
        double area_eps = 1.0e-14
    ) const;

private:
    /**
     * @brief 根据三个节点计算三角形面积
     *
     * @param a 第一个节点
     * @param b 第二个节点
     * @param c 第三个节点
     * @return 三角形面积
     */
    static double triangle_area(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

private:
    /**
     * @brief 网格来源类型
     */
    TriangleMeshSourceType source_type_ = TriangleMeshSourceType::Unknown;

    /**
     * @brief 结构网格生成信息
     */
    TriangleMeshStructuredInfo structured_info_;

    TriangleMeshGeneratorInfo generator_info_;
};

} // namespace OpenCAX

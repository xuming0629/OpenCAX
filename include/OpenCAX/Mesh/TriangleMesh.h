#pragma once

/**
 * @file TriangleMesh.h
 * @brief OpenCAX 二维三角形网格类声明
 *
 * TriangleMesh 是 Mesh 的派生类，专门用于管理 Triangle3 单元。
 *
 * 当前支持：
 *
 * - 内置结构矩形三角形网格
 * - 外部非结构三角形网格
 * - Gmsh / Netgen / TetGen 等第三方网格器生成后的统一接入
 *
 * 设计原则：
 *
 * - TriangleMesh 只负责 OpenCAX 自己的网格数据结构
 * - TriangleMesh 不直接依赖 gmsh.h / netgen / tetgen
 * - 第三方库生成的 points + triangles 统一通过 create_unstructured() 接入
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>

#include <array>
#include <string>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 三角形网格来源类型
 */
enum class TriangleMeshSourceType
{
    Unknown,        ///< 未知来源
    Structured,     ///< OpenCAX 内置结构网格
    Unstructured,   ///< 非结构网格
    GmshStructured, ///< Gmsh Transfinite 结构网格
    GmshUnstructured ///< Gmsh 非结构网格
};

/**
 * @brief 结构三角形网格信息
 */
struct OpenCAX_API TriangleMeshStructuredInfo
{
    int nx = 0;
    int ny = 0;

    double xmin = 0.0;
    double xmax = 0.0;

    double ymin = 0.0;
    double ymax = 0.0;

    bool alternate_diagonal = false;
};

/**
 * @brief 二维三角形网格类
 */
class OpenCAX_API TriangleMesh : public Mesh
{
public:
    /**
     * @brief 默认构造
     */
    TriangleMesh();

    /**
     * @brief 创建结构矩形三角形网格
     *
     * @param xmin x 最小值
     * @param xmax x 最大值
     * @param ymin y 最小值
     * @param ymax y 最大值
     * @param nx x 方向划分数量
     * @param ny y 方向划分数量
     * @param alternate_diagonal 是否交错对角线
     * @return TriangleMesh
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
     * @param points 节点坐标
     * @param triangles 三角形连接关系
     * @return TriangleMesh
     *
     * @note
     * points 的编号从 0 开始。
     * triangles 中的节点 ID 必须引用 points 中的合法节点。
     */
    static TriangleMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 3>>& triangles
    );

    /**
     * @brief 获取网格来源类型
     */
    TriangleMeshSourceType source_type() const;

    /**
     * @brief 设置网格来源类型
     */
    void set_source_type(
        TriangleMeshSourceType type
    );

    /**
     * @brief 设置网格名称
     */
    void set_name(
        const std::string& name
    );

    /**
     * @brief 设置网格来源字符串
     */
    void set_source(
        const std::string& source
    );

    /**
     * @brief 判断是否为结构网格
     */
    bool is_structured() const;

    /**
     * @brief 判断是否为非结构网格
     */
    bool is_unstructured() const;

    /**
     * @brief 获取结构网格信息
     */
    const TriangleMeshStructuredInfo& structured_info() const;

    /**
     * @brief 设置结构网格信息
     */
    void set_structured_info(
        const TriangleMeshStructuredInfo& info
    );

    /**
     * @brief 计算三角形面积
     */
    static double triangle_area(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

    /**
     * @brief 计算某个单元面积
     */
    double area(
        int cell_id
    ) const;

    /**
     * @brief 计算总面积
     */
    double total_area() const;

    /**
     * @brief 计算某个三角形单元质心
     */
    std::array<double, 3> centroid(
        int cell_id
    ) const;

    /**
     * @brief 计算包围盒
     *
     * 返回：
     *
     * {xmin, xmax, ymin, ymax, zmin, zmax}
     */
    std::array<double, 6> bounds() const;

    /**
     * @brief 检查三角形网格合法性
     */
    bool validate(
        std::string* error_message = nullptr,
        double area_eps = 1.0e-14
    ) const;

private:
    TriangleMeshSourceType source_type_ = TriangleMeshSourceType::Unknown;

    TriangleMeshStructuredInfo structured_info_;
};

} // namespace OpenCAX
#pragma once

/**
 * @file QuadMesh.h
 * @brief OpenCAX Mesh2 二维四边形网格类声明
 *
 * QuadMesh 是 Mesh 的派生类，专门用于二维四边形网格。
 *
 * 当前支持：
 *
 * - 结构矩形区域 Quad4 网格生成
 * - 非结构 Quad4 网格创建
 * - Quad4 / Quad8 / Quad9 面积计算
 * - Quad4 / Quad8 / Quad9 质心计算
 * - 四边形网格合法性检查
 *
 * @note
 * QuadMesh 只负责四边形网格的数据管理和基础几何计算。
 * 全局拓扑关系，例如 edge、cell2edge、edge2cell、node2cell、
 * cell2cell、boundary_edges 等，应由 MeshTopology 构建。
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh2/Mesh.h>

#include <array>
#include <string>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 二维四边形网格类
 *
 * QuadMesh 继承自 Mesh。
 *
 * 主要支持：
 *
 * - CellType::Quad4
 * - CellType::Quad8
 * - CellType::Quad9
 */
class OpenCAX_API QuadMesh : public Mesh
{
public:
    /**
     * @brief 构造空四边形网格
     */
    QuadMesh();

    /**
     * @brief 创建矩形区域上的结构 Quad4 网格
     *
     * @param xmin X 方向最小坐标
     * @param xmax X 方向最大坐标
     * @param ymin Y 方向最小坐标
     * @param ymax Y 方向最大坐标
     * @param nx X 方向划分数量
     * @param ny Y 方向划分数量
     * @return 结构四边形网格
     */
    static QuadMesh create_structured_rectangle(
        double xmin,
        double xmax,
        double ymin,
        double ymax,
        int nx,
        int ny
    );

    /**
     * @brief 创建非结构 Quad4 网格
     *
     * @param points 节点坐标数组，每个点为 {x,y,z}
     * @param quads 四边形连接关系，每个单元为 {n0,n1,n2,n3}
     * @return 非结构四边形网格
     */
    static QuadMesh create_unstructured(
        const std::vector<std::array<double, 3>>& points,
        const std::vector<std::array<int, 4>>& quads
    );

    /**
     * @brief 设置结构网格信息
     */
    void set_structured_info(
        const StructuredGridInfo2D& info
    );

    /**
     * @brief 获取结构网格信息
     */
    const StructuredGridInfo2D& structured_info() const;

    /**
     * @brief 计算三角形面积
     *
     * 四边形面积计算时会用到该辅助函数。
     */
    static double triangle_area(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c
    );

    /**
     * @brief 计算四边形面积
     *
     * 默认按对角线 a-c 拆成两个三角形：
     *
     * - triangle(a,b,c)
     * - triangle(a,c,d)
     */
    static double quad_area(
        const MeshNode& a,
        const MeshNode& b,
        const MeshNode& c,
        const MeshNode& d
    );

    /**
     * @brief 计算指定四边形单元面积
     */
    double area(
        int cell_id
    ) const;

    /**
     * @brief 计算整个四边形网格总面积
     */
    double total_area() const;

    /**
     * @brief 计算指定四边形单元质心
     */
    std::array<double, 3> centroid(
        int cell_id
    ) const;

    /**
     * @brief 四边形网格合法性检查
     *
     * @param error_message 错误信息输出
     * @param area_eps 面积容差
     */
    bool validate(
        std::string* error_message,
        double area_eps
    ) const;

    /**
     * @brief 默认四边形网格合法性检查
     */
    bool validate(
        std::string* error_message = nullptr
    ) const override;

private:
    /**
     * @brief 二维结构网格信息
     */
    StructuredGridInfo2D structured_info_;
};

} // namespace OpenCAX
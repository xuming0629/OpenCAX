#pragma once

/**
 * @file GmshTriangleMeshGenerator.h
 * @brief 基于 Gmsh 的二维三角形网格生成器
 *
 * 当前只负责二维矩形区域 Triangle3 网格生成。
 *
 * 支持：
 *
 * - Gmsh 非结构三角形网格
 * - Gmsh Transfinite 结构三角形网格
 *
 * 注意：
 *
 * 本类依赖 gmsh.h。
 * TriangleMesh 本身不依赖 gmsh.h。
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <string>

namespace OpenCAX
{

/**
 * @brief Gmsh 三角形网格生成参数
 */
struct OpenCAX_API GmshTriangleMeshOptions
{
    /**
     * @brief 非结构网格尺寸
     */
    double mesh_size = 0.1;

    /**
     * @brief 是否优化网格
     */
    bool optimize = true;

    /**
     * @brief 是否输出 Gmsh 日志
     */
    bool verbose = false;

    /**
     * @brief Gmsh 二维网格算法
     *
     * 常用：
     *
     * 5 = Delaunay
     * 6 = Frontal-Delaunay
     * 8 = Frontal-Delaunay for Quads
     */
    int algorithm = 6;

    /**
     * @brief 模型名称
     */
    std::string model_name = "OpenCAX_Gmsh_TriangleMesh";
};

/**
 * @brief Gmsh 三角形网格生成器
 */
class OpenCAX_API GmshTriangleMeshGenerator
{
public:
    /**
     * @brief 创建 Gmsh 非结构矩形三角形网格
     */
    static TriangleMesh create_unstructured_rectangle(
        double xmin,
        double xmax,
        double ymin,
        double ymax,
        const GmshTriangleMeshOptions& options = GmshTriangleMeshOptions()
    );

    /**
     * @brief 创建 Gmsh 结构矩形三角形网格
     *
     * 使用 Gmsh Transfinite Surface。
     *
     * nx / ny 表示单元划分数量。
     *
     * 注意：
     *
     * Gmsh 的 TransfiniteCurve 参数是点数，
     * 所以内部会使用 nx + 1 和 ny + 1。
     */
    static TriangleMesh create_structured_rectangle(
        double xmin,
        double xmax,
        double ymin,
        double ymax,
        int nx,
        int ny,
        const GmshTriangleMeshOptions& options = GmshTriangleMeshOptions()
    );

    /**
     * @brief 释放 Gmsh
     */
    static void finalize();

private:
    GmshTriangleMeshGenerator() = delete;
};

} // namespace OpenCAX
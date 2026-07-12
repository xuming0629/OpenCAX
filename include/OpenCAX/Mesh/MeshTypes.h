#pragma once

/**
 * @file MeshTypes.h
 * @brief OpenCAX 网格模块基础数据类型定义
 *
 * 本文件定义 OpenCAX Mesh 模块最基础的数据结构。
 *
 * 主要包括：
 *
 * - MeshDimension    : 网格拓扑维度
 * - CellType         : 网格单元类型
 * - MeshOrder        : 网格单元阶次
 * - MeshElementFamily: 单元族类型
 * - MeshSourceType   : 网格来源类型
 * - MeshNode         : 网格节点
 * - MeshCell         : 网格单元
 * - MeshInfo         : 网格描述信息
 * - StructuredGridInfo2D : 二维结构网格参数
 * - StructuredGridInfo3D : 三维结构网格参数
 *
 * 设计目标：
 *
 * 1. MeshTypes 只负责定义数据结构，不负责算法。
 *
 * 2. Mesh:
 *
 *    - 保存节点
 *    - 保存单元
 *    - 保存网格元信息
 *
 * 3. MeshTopology:
 *
 *    - edge2node
 *    - cell2edge
 *    - edge2cell
 *    - node2cell
 *    - cell2cell
 *
 * 4. Meshing:
 *
 *    - 结构网格生成
 *    - Gmsh 网格导入
 *    - 网格加密
 *    - 高阶网格转换
 *
 * 5. FEM:
 *
 *    - 使用 MeshCell::node_ids
 *    - 使用 CellType 判断单元类型
 *    - 使用 order 判断有限元阶次
 *
 * @note
 * 高阶单元统一存储所有节点。
 *
 * 例如：
 *
 * Triangle6:
 *
 * ```
 * node_ids =
 * {
 *     v0,
 *     v1,
 *     v2,
 *     m01,
 *     m12,
 *     m20
 * };
 * ```
 *
 * 其中：
 *
 * - v0/v1/v2 为角点
 * - m01/m12/m20 为边中点
 *
 * @note
 * MeshTopology 默认按照角点构建数学拓扑。
 * 高阶节点主要用于：
 *
 * - FEM 插值
 * - 高阶积分
 * - VTK 显示
 *
 */

#include <string>
#include <vector>

namespace OpenCAX
{

    /**
     * @brief 网格拓扑维度
     *
     * MeshDimension 表示单元的拓扑维度，而不是坐标维度。
     *
     * 示例：
     *
     * 三维空间中的三角曲面：
     *
     * - 坐标:
     *   (x,y,z)
     *
     * - 拓扑维度:
     *   Dim2
     *
     * 因为它仍然是二维面单元。
     */
    enum class MeshDimension
    {
        /**
         * @brief 未知维度
         */
        Unknown = -1,

        /**
         * @brief 零维网格
         *
         * 例如：
         *
         * - 点
         */
        Dim0 = 0,

        /**
         * @brief 一维网格
         *
         * 例如：
         *
         * - Line2
         * - Line3
         */
        Dim1 = 1,

        /**
         * @brief 二维网格
         *
         * 例如：
         *
         * - Triangle
         * - Quad
         */
        Dim2 = 2,

        /**
         * @brief 三维网格
         *
         * 例如：
         *
         * - Tetra
         * - Hexa
         */
        Dim3 = 3
    };

    /**
     * @brief 网格单元类型
     *
     * CellType 描述有限元或者几何离散中的单元类型。
     *
     * 包括：
     *
     * - 一维单元
     * - 二维单元
     * - 三维单元
     * - 一阶单元
     * - 二阶高阶单元
     *
     * 命名规则：
     *
     * ```
     * 类型 + 节点数量
     * ```
     *
     * 例如：
     *
     * Triangle3:
     *
     * - Triangle
     * - 3 个节点
     * - 一阶三角形
     *
     * Triangle6:
     *
     * - Triangle
     * - 6 个节点
     * - 二阶三角形
     */
    enum class CellType
    {
        /**
         * @brief 未知单元
         */
        Unknown = 0,

        /**
         * @brief 点单元
         *
         * 一个节点。
         */
        Vertex1,

        /**
         * @brief 二节点线单元
         */
        Line2,

        /**
         * @brief 三节点二阶线单元
         *
         * 节点：
         *
         * ```
         * v0 - m01 - v1
         * ```
         */
        Line3,

        /**
         * @brief 三节点一阶三角形
         *
         * 节点：
         *
         * ```
         *      v2
         *     / \
         *    /   \
         *   v0---v1
         * ```
         */
        Triangle3,

        /**
         * @brief 六节点二阶三角形
         *
         * 节点：
         *
         * ```
         *      v2
         *     / \
         *   m20 m12
         *   /     \
         * v0--m01--v1
         * ```
         */
        Triangle6,

        /**
         * @brief 四节点四边形
         *
         * 一阶 Quad 单元。
         */
        Quad4,

        /**
         * @brief 八节点二阶四边形
         */
        Quad8,

        /**
         * @brief 九节点二阶四边形
         *
         * 包含中心节点。
         */
        Quad9,

        /**
         * @brief 四节点四面体
         */
        Tetra4,

        /**
         * @brief 十节点二阶四面体
         */
        Tetra10,

        /**
         * @brief 八节点六面体
         */
        Hexa8,

        /**
         * @brief 二十节点二阶六面体
         */
        Hexa20,

        /**
         * @brief 二十七节点二阶六面体
         */
        Hexa27,

        /**
         * @brief 六节点三棱柱
         */
        Prism6,

        /**
         * @brief 十五节点二阶三棱柱
         */
        Prism15,

        /**
         * @brief 五节点金字塔
         */
        Pyramid5,

        /**
         * @brief 十三节点二阶金字塔
         */
        Pyramid13
    };

    /**
     * @brief 单元阶次
     *
     * 用于 FEM 插值阶次判断。
     *
     * 示例：
     *
     * - Triangle3 -> Linear
     * - Triangle6 -> Quadratic
     */
    enum class MeshOrder
    {
        /**
         * @brief 未知阶次
         */
        Unknown = 0,

        /**
         * @brief 一阶单元
         */
        Linear = 1,

        /**
         * @brief 二阶高阶单元
         */
        Quadratic = 2
    };

    /**
     * @brief 单元族类型
     *
     * 用于按照几何族分类。
     *
     * 例如：
     *
     * Triangle3/Triangle6:
     *
     * ```
     * MeshElementFamily::Triangle
     * ```
     */
    enum class MeshElementFamily
    {
        Unknown,

        Vertex,

        Line,

        Triangle,

        Quadrilateral,

        Tetrahedron,

        Hexahedron,

        Prism,

        Pyramid,

        Mixed
    };

    /**
     * @brief 网格来源类型
     *
     * 描述网格如何生成。
     */
    enum class MeshSourceType
    {
        /**
         * @brief 未知来源
         */
        Unknown,

        /**
         * @brief OpenCAX 内置结构网格
         */
        Structured,

        /**
         * @brief 非结构网格
         */
        Unstructured,

        /**
         * @brief Gmsh 生成
         */
        Gmsh,

        /**
         * @brief TetGen 生成
         */
        TetGen,

        /**
         * @brief 外部导入
         */
        Imported,

        /**
         * @brief 网格加密产生
         */
        Refined,

        /**
         * @brief 高阶转换产生
         *
         * 例如：
         *
         * Triangle3
         *
         * 转换为：
         *
         * Triangle6
         */
        HighOrderConverted
    };

    /**
     * @brief 网格节点
     *
     * MeshNode 表示有限元节点或者几何离散节点。
     *
     * 所有节点统一采用三维坐标：
     *
     * - 2D 网格:
     *
     *   z = 0
     *
     * - 曲面网格:
     *
     *   x/y/z
     *
     * - 3D 网格:
     *
     *   x/y/z
     */
    struct MeshNode
    {
        /**
         * @brief 节点编号
         *
         * 从 0 开始连续编号。
         */
        int id = -1;

        /**
         * @brief 空间坐标 X
         */
        double x = 0.0;

        /**
         * @brief 空间坐标 Y
         */
        double y = 0.0;

        /**
         * @brief 空间坐标 Z
         */
        double z = 0.0;

        /**
         * @brief 物理分组编号
         *
         * 对应：
         *
         * - Gmsh Physical Group
         * - CAD 拓扑区域
         */
        int physical_id = -1;

        /**
         * @brief 边界编号
         *
         * 用于：
         *
         * - Dirichlet 边界
         * - Neumann 边界
         */
        int boundary_id = -1;

        /**
         * @brief 几何实体编号
         *
         * 对应 CAD:
         *
         * - Vertex
         * - Edge
         * - Face
         */
        int geometry_id = -1;
    };

    /**
     * @brief 网格单元
     *
     * MeshCell 保存一个有限元单元的完整信息。
     */
    struct MeshCell
    {
        /**
         * @brief 单元编号
         */
        int id = -1;

        /**
         * @brief 单元类型
         */
        CellType type = CellType::Unknown;

        /**
         * @brief 单元节点连接关系
         *
         * 存储该单元包含的节点编号。
         *
         * 示例：
         *
         * Triangle3:
         *
         * ```
         * {0,1,2}
         * ```
         *
         * Triangle6:
         *
         * ```
         * {0,1,2,3,4,5}
         * ```
         */
        std::vector<int> node_ids;

        /**
         * @brief 单元阶次
         *
         * 例如：
         *
         * - 1 一阶
         * - 2 二阶
         */
        int order = 0;

        /**
         * @brief 物理区域编号
         *
         * 用于：
         *
         * - 材料映射
         * - 多物理区域
         */
        int physical_id = -1;

        /**
         * @brief 材料编号
         *
         * FEM:
         *
         * ```
         * material_id -> Material
         * ```
         */
        int material_id = -1;

        /**
         * @brief 区域编号
         *
         * 用于多个几何区域管理。
         */
        int region_id = -1;

        /**
         * @brief 几何实体编号
         */
        int geometry_id = -1;
    };

    /**
     * @brief 网格信息
     *
     * 保存整个 Mesh 的描述信息。
     */
    struct MeshInfo
    {
        /**
         * @brief 网格名称
         */
        std::string name = "OpenCAXMesh";

        /**
         * @brief 网格来源
         *
         * 示例：
         *
         * - Structured
         * - Gmsh
         * - TetGen
         */
        std::string source = "OpenCAX";

        /**
         * @brief 网格拓扑维度
         */
        MeshDimension dimension = MeshDimension::Unknown;

        /**
         * @brief 网格生成来源
         */
        MeshSourceType source_type = MeshSourceType::Unknown;

        /**
         * @brief 是否结构网格
         */
        bool structured = false;

        /**
         * @brief 是否高阶网格
         */
        bool high_order = false;
    };

    /**
     * @brief 二维结构网格参数
     *
     * 用于保存矩形结构网格生成信息。
     */
    struct StructuredGridInfo2D
    {
        int nx = 0;
        int ny = 0;

        double xmin = 0.0;
        double xmax = 0.0;

        double ymin = 0.0;
        double ymax = 0.0;

        /**
         * @brief 是否交错对角线
         *
         * Triangle 网格生成时使用。
         */
        bool alternate_diagonal = false;
    };

    /**
     * @brief 三维结构网格参数
     *
     * 用于六面体、四面体结构网格生成。
     */
    struct StructuredGridInfo3D
    {
        int nx = 0;
        int ny = 0;
        int nz = 0;

        double xmin = 0.0;
        double xmax = 0.0;

        double ymin = 0.0;
        double ymax = 0.0;

        double zmin = 0.0;
        double zmax = 0.0;
    };

    /**
     * @brief CellType 字符串转换
     */
    const char *to_string(CellType type);

    /**
     * @brief MeshDimension 字符串转换
     */
    const char *to_string(MeshDimension dimension);

    /**
     * @brief MeshSourceType 字符串转换
     */
    const char *to_string(MeshSourceType source_type);

} // namespace OpenCAX
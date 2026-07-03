#pragma once

/**
 * @file MeshTypes.h
 * @brief OpenCAX 网格模块基础数据类型定义
 *
 * 本文件定义 OpenCAX Mesh 模块中最基础的数据结构和枚举类型。
 *
 * 主要包括：
 *
 * - CellType      : 网格单元类型
 * - MeshDimension : 网格拓扑维度
 * - MeshNode      : 网格节点
 * - MeshCell      : 网格单元
 * - MeshFace      : 网格面
 * - MeshInfo      : 网格基础信息
 *
 * 这些类型是 Mesh、MeshTopology、CellTopology、FEM、IO、Post 等模块的基础依赖。
 */

#include <string>
#include <vector>

namespace OpenCAX
{

/**
 * @brief 网格单元类型
 *
 * CellType 用于描述 MeshCell 的拓扑类型。
 *
 * 设计目标：
 *
 * - 支持 0D 点单元
 * - 支持 1D 边网格
 * - 支持 2D 面网格
 * - 支持 3D 体网格
 * - 支持后续 FEM 高阶单元扩展
 *
 * 常见一阶单元：
 *
 * - Vertex1   : 单节点点单元
 * - Line2     : 二节点线单元
 * - Triangle3 : 三节点一阶三角形单元
 * - Quad4     : 四节点一阶四边形单元
 * - Tetra4    : 四节点一阶四面体单元
 * - Hexa8     : 八节点一阶六面体单元
 * - Prism6    : 六节点三棱柱单元
 * - Pyramid5  : 五节点金字塔单元
 *
 * 常见二阶单元：
 *
 * - Line3     : 三节点二阶线单元
 * - Triangle6 : 六节点二阶三角形单元
 * - Quad8     : 八节点二阶四边形单元
 * - Quad9     : 九节点二阶四边形单元
 * - Tetra10   : 十节点二阶四面体单元
 * - Hexa20    : 二十节点二阶六面体单元
 * - Hexa27    : 二十七节点二阶六面体单元
 * - Prism15   : 十五节点二阶三棱柱单元
 * - Pyramid13 : 十三节点二阶金字塔单元
 *
 * @note
 * 当前 MeshTopology 构建拓扑关系时，高阶单元默认只使用角点拓扑。
 * 例如 Triangle6 仍按 Triangle3 的三条边处理。
 *
 * @note
 * 高阶边节点、面节点、体内部节点等信息应由 FEMElement 或后续专门的
 * HighOrderTopology 工具处理。
 */
enum class CellType
{
    /**
     * @brief 未知或未支持的单元类型
     */
    Unknown = 0,

    /**
     * @brief 点单元，1 个节点
     */
    Vertex1,

    /**
     * @brief 一阶线单元，2 个节点
     */
    Line2,

    /**
     * @brief 二阶线单元，3 个节点
     */
    Line3,

    /**
     * @brief 一阶三角形单元，3 个节点
     */
    Triangle3,

    /**
     * @brief 二阶三角形单元，6 个节点
     */
    Triangle6,

    /**
     * @brief 一阶四边形单元，4 个节点
     */
    Quad4,

    /**
     * @brief 二阶四边形单元，8 个节点
     */
    Quad8,

    /**
     * @brief 二阶四边形单元，9 个节点
     */
    Quad9,

    /**
     * @brief 一阶四面体单元，4 个节点
     */
    Tetra4,

    /**
     * @brief 二阶四面体单元，10 个节点
     */
    Tetra10,

    /**
     * @brief 一阶六面体单元，8 个节点
     */
    Hexa8,

    /**
     * @brief 二阶六面体单元，20 个节点
     */
    Hexa20,

    /**
     * @brief 二阶六面体单元，27 个节点
     */
    Hexa27,

    /**
     * @brief 一阶三棱柱单元，6 个节点
     */
    Prism6,

    /**
     * @brief 二阶三棱柱单元，15 个节点
     */
    Prism15,

    /**
     * @brief 一阶金字塔单元，5 个节点
     */
    Pyramid5,

    /**
     * @brief 二阶金字塔单元，13 个节点
     */
    Pyramid13
};

/**
 * @brief 网格维度
 *
 * MeshDimension 描述网格的拓扑维度，而不是几何坐标维度。
 *
 * 示例：
 *
 * - Dim0 : 点网格，例如离散点单元
 * - Dim1 : 线网格，例如边界线、梁单元、曲线网格
 * - Dim2 : 面网格，例如三角形网格、四边形网格、曲面网格
 * - Dim3 : 体网格，例如四面体网格、六面体网格、体域网格
 *
 * @note
 * 即使是 Dim2 网格，节点仍然可以存储 x/y/z。
 * 例如三维空间中的曲面三角网格，它的拓扑维度是 Dim2，
 * 但几何坐标维度是三维。
 */
enum class MeshDimension
{
    /**
     * @brief 未知维度
     */
    Unknown = -1,

    /**
     * @brief 零维网格
     */
    Dim0 = 0,

    /**
     * @brief 一维网格
     */
    Dim1 = 1,

    /**
     * @brief 二维网格
     */
    Dim2 = 2,

    /**
     * @brief 三维网格
     */
    Dim3 = 3
};

/**
 * @brief 网格节点
 *
 * MeshNode 表示一个网格节点。
 *
 * 所有网格统一使用三维坐标：
 *
 * - 0D 网格：使用 x/y/z 表示空间点
 * - 1D 网格：可只使用 x，或使用 x/y/z 表示空间曲线
 * - 2D 平面网格：使用 x/y，z = 0
 * - 2D 曲面网格：使用 x/y/z
 * - 3D 体网格：使用 x/y/z
 *
 * 字段说明：
 *
 * - id          : 节点编号
 * - x/y/z       : 节点坐标
 * - physical_id : 物理分组编号，通常对应 Gmsh Physical Group
 * - boundary_id : 边界编号，用于 FEM 边界条件
 */
struct MeshNode
{
    /**
     * @brief 节点 ID
     *
     * 通常从 0 开始连续编号。
     */
    int id = -1;

    /**
     * @brief X 坐标
     */
    double x = 0.0;

    /**
     * @brief Y 坐标
     */
    double y = 0.0;

    /**
     * @brief Z 坐标
     *
     * 对于二维平面网格，通常为 0。
     */
    double z = 0.0;

    /**
     * @brief 物理分组 ID
     *
     * 可用于记录 Gmsh Physical Group。
     */
    int physical_id = -1;

    /**
     * @brief 边界 ID
     *
     * 用于 FEM 中施加 Dirichlet / Neumann 等边界条件。
     */
    int boundary_id = -1;
};

/**
 * @brief 网格单元
 *
 * MeshCell 表示一个网格单元。
 *
 * 通过 type 描述单元类型，
 * 通过 node_ids 描述单元与节点的连接关系。
 *
 * 示例：
 *
 * Triangle3:
 *
 * @code
 * type = CellType::Triangle3;
 * node_ids = {0, 1, 2};
 * @endcode
 *
 * Quad4:
 *
 * @code
 * type = CellType::Quad4;
 * node_ids = {0, 1, 2, 3};
 * @endcode
 *
 * Tetra4:
 *
 * @code
 * type = CellType::Tetra4;
 * node_ids = {0, 1, 2, 3};
 * @endcode
 *
 * Hexa8:
 *
 * @code
 * type = CellType::Hexa8;
 * node_ids = {0, 1, 2, 3, 4, 5, 6, 7};
 * @endcode
 *
 * 字段说明：
 *
 * - id          : 单元编号
 * - type        : 单元类型
 * - node_ids    : 单元包含的节点 ID
 * - order       : 单元阶次，一阶为 1，二阶为 2
 * - physical_id : 物理分组 ID，通常对应 Gmsh Physical Group
 * - material_id : 材料 ID，用于 FEM 材料映射
 * - region_id   : 几何区域 ID，例如不同体域、面域
 */
struct MeshCell
{
    /**
     * @brief 单元 ID
     *
     * 通常从 0 开始连续编号。
     */
    int id = -1;

    /**
     * @brief 单元类型
     */
    CellType type = CellType::Unknown;

    /**
     * @brief 单元节点连接关系
     *
     * 存储构成该单元的节点 ID。
     *
     * 节点顺序应遵循 OpenCAX 统一约定，
     * 后续可与 Gmsh / VTK / FEM 单元顺序进行映射。
     */
    std::vector<int> node_ids;

    /**
     * @brief 单元阶次
     *
     * 一阶单元：
     *
     * - Vertex1
     * - Line2
     * - Triangle3
     * - Quad4
     * - Tetra4
     * - Hexa8
     * - Prism6
     * - Pyramid5
     *
     * 二阶单元：
     *
     * - Line3
     * - Triangle6
     * - Quad8
     * - Quad9
     * - Tetra10
     * - Hexa20
     * - Hexa27
     * - Prism15
     * - Pyramid13
     */
    int order = 1;

    /**
     * @brief 物理分组 ID
     *
     * 常用于记录 Gmsh Physical Group。
     */
    int physical_id = -1;

    /**
     * @brief 材料 ID
     *
     * FEM 中用于将单元映射到材料参数。
     */
    int material_id = -1;

    /**
     * @brief 区域 ID
     *
     * 可用于区分几何区域、体域、面域。
     */
    int region_id = -1;
};

/**
 * @brief 网格面
 *
 * MeshFace 表示三维体网格中的一个拓扑面。
 *
 * MeshFace 通常由 MeshTopology 构建，用于描述：
 *
 * - face2node : 面到节点
 * - face2cell : 面到相邻单元
 * - cell2face : 单元到面
 *
 * 示例：
 *
 * Tetra4 的面通常是 Triangle3。
 *
 * @code
 * type = CellType::Triangle3;
 * node_ids = {0, 1, 2};
 * cell_ids = {0};
 * @endcode
 *
 * Hexa8 的面通常是 Quad4。
 *
 * @code
 * type = CellType::Quad4;
 * node_ids = {0, 1, 2, 3};
 * cell_ids = {0, 1};
 * @endcode
 *
 * @note
 * MeshFace 是拓扑层结构，主要用于全局邻接关系和边界识别。
 * 对于 FEM 中需要面的局部方向、法向、Jacobian 等信息的场景，
 * 应由具体 FEMElement 或 CellTopology 的扩展接口处理。
 */
struct MeshFace
{
    /**
     * @brief 面 ID
     *
     * 通常从 0 开始连续编号。
     */
    int id = -1;

    /**
     * @brief 面类型
     *
     * 常见取值：
     *
     * - Triangle3
     * - Quad4
     */
    CellType type = CellType::Unknown;

    /**
     * @brief 面节点连接关系
     *
     * 存储构成该面的节点 ID。
     */
    std::vector<int> node_ids;

    /**
     * @brief 相邻单元 ID 列表
     *
     * 边界面通常只有 1 个相邻单元。
     * 内部面通常有 2 个相邻单元。
     */
    std::vector<int> cell_ids;

    /**
     * @brief 是否为边界面
     */
    bool boundary = false;

    /**
     * @brief 物理分组 ID
     *
     * 可用于记录边界面的物理分组。
     */
    int physical_id = -1;

    /**
     * @brief 边界 ID
     *
     * 用于 FEM 中施加边界条件。
     */
    int boundary_id = -1;
};

/**
 * @brief 网格基础信息
 *
 * MeshInfo 保存网格级别的元信息。
 *
 * 包括：
 *
 * - name      : 网格名称
 * - source    : 网格来源，例如 Gmsh、TetGen、Structured、Imported
 * - unit      : 网格单位
 * - dimension : 网格拓扑维度
 */
struct MeshInfo
{
    /**
     * @brief 网格名称
     */
    std::string name;

    /**
     * @brief 网格来源
     *
     * 示例：
     *
     * - "Structured"
     * - "Gmsh"
     * - "TetGen"
     * - "Netgen"
     * - "STL"
     * - "VTK"
     */
    std::string source;

    /**
     * @brief 网格单位
     *
     * 默认使用 m。
     */
    std::string unit = "m";

    /**
     * @brief 网格拓扑维度
     */
    MeshDimension dimension = MeshDimension::Unknown;
};

} // namespace OpenCAX
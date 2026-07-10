/**
 * @file Mesh.cpp
 * @brief OpenCAX Mesh2 通用网格基础容器实现
 *
 * Mesh 是 OpenCAX Mesh2 模块中的基础网格容器。
 *
 * 本文件主要实现：
 *
 * - 网格节点管理
 * - 网格单元管理
 * - 网格基础信息管理
 * - 网格合法性检查
 * - 网格包围盒计算
 * - 网格阶次判断
 * - 网格单元类型统计
 *
 * @note
 * Mesh 只负责保存 nodes / cells / info。
 * 全局拓扑关系，例如 edge、face、cell2cell、boundary 等，
 * 应由 MeshTopology 单独构建。
 */

#include <OpenCAX/Mesh2/Mesh.h>
#include <OpenCAX/Mesh2/CellTopology.h>

#include <algorithm>
#include <limits>
#include <memory>
#include <set>
#include <stdexcept>

namespace OpenCAX
{

/**
 * @brief 将 CellType 转换为字符串
 *
 * 该函数转发到 CellTopology::to_string。
 */
const char* to_string(
    CellType type
)
{
    return CellTopology::to_string(type);
}

/**
 * @brief 将 MeshDimension 转换为字符串
 */
const char* to_string(
    MeshDimension d
)
{
    switch (d)
    {
    case MeshDimension::Dim0:
        return "Dim0";

    case MeshDimension::Dim1:
        return "Dim1";

    case MeshDimension::Dim2:
        return "Dim2";

    case MeshDimension::Dim3:
        return "Dim3";

    case MeshDimension::Unknown:
    default:
        return "Unknown";
    }
}

/**
 * @brief 将 MeshSourceType 转换为字符串
 */
const char* to_string(
    MeshSourceType s
)
{
    switch (s)
    {
    case MeshSourceType::Structured:
        return "Structured";

    case MeshSourceType::Unstructured:
        return "Unstructured";

    case MeshSourceType::Gmsh:
        return "Gmsh";

    case MeshSourceType::TetGen:
        return "TetGen";

    case MeshSourceType::Imported:
        return "Imported";

    case MeshSourceType::Refined:
        return "Refined";

    case MeshSourceType::HighOrderConverted:
        return "HighOrderConverted";

    case MeshSourceType::Unknown:
    default:
        return "Unknown";
    }
}

/**
 * @brief 清空网格数据
 *
 * 会重置：
 *
 * - MeshInfo
 * - nodes_
 * - cells_
 */
void Mesh::clear()
{
    info_ = MeshInfo{};
    nodes_.clear();
    cells_.clear();
}

/**
 * @brief 获取网格信息
 */
MeshInfo& Mesh::info()
{
    return info_;
}

/**
 * @brief 获取只读网格信息
 */
const MeshInfo& Mesh::info() const
{
    return info_;
}

/**
 * @brief 设置网格名称
 */
void Mesh::set_name(
    const std::string& name
)
{
    info_.name = name;
}

/**
 * @brief 设置网格来源描述
 */
void Mesh::set_source(
    const std::string& source
)
{
    info_.source = source;
}

/**
 * @brief 设置网格维度
 */
void Mesh::set_dimension(
    MeshDimension dim
)
{
    info_.dimension = dim;
}

/**
 * @brief 设置网格来源类型
 */
void Mesh::set_source_type(
    MeshSourceType type
)
{
    info_.source_type = type;
}

/**
 * @brief 获取节点数组
 */
std::vector<MeshNode>& Mesh::nodes()
{
    return nodes_;
}

/**
 * @brief 获取只读节点数组
 */
const std::vector<MeshNode>& Mesh::nodes() const
{
    return nodes_;
}

/**
 * @brief 获取指定节点
 *
 * @throws std::out_of_range 当节点 ID 非法时抛出异常
 */
MeshNode& Mesh::node(
    int id
)
{
    if (!valid_node_id(id))
    {
        throw std::out_of_range(
            "Mesh::node: invalid node id."
        );
    }

    return nodes_[static_cast<std::size_t>(id)];
}

/**
 * @brief 获取指定只读节点
 *
 * @throws std::out_of_range 当节点 ID 非法时抛出异常
 */
const MeshNode& Mesh::node(
    int id
) const
{
    if (!valid_node_id(id))
    {
        throw std::out_of_range(
            "Mesh::node: invalid node id."
        );
    }

    return nodes_[static_cast<std::size_t>(id)];
}

/**
 * @brief 获取单元数组
 */
std::vector<MeshCell>& Mesh::cells()
{
    return cells_;
}

/**
 * @brief 获取只读单元数组
 */
const std::vector<MeshCell>& Mesh::cells() const
{
    return cells_;
}

/**
 * @brief 获取指定单元
 *
 * @throws std::out_of_range 当单元 ID 非法时抛出异常
 */
MeshCell& Mesh::cell(
    int id
)
{
    if (!valid_cell_id(id))
    {
        throw std::out_of_range(
            "Mesh::cell: invalid cell id."
        );
    }

    return cells_[static_cast<std::size_t>(id)];
}

/**
 * @brief 获取指定只读单元
 *
 * @throws std::out_of_range 当单元 ID 非法时抛出异常
 */
const MeshCell& Mesh::cell(
    int id
) const
{
    if (!valid_cell_id(id))
    {
        throw std::out_of_range(
            "Mesh::cell: invalid cell id."
        );
    }

    return cells_[static_cast<std::size_t>(id)];
}

/**
 * @brief 添加普通节点
 *
 * @return 新节点 ID
 */
int Mesh::add_node(
    double x,
    double y,
    double z
)
{
    const int id =
        static_cast<int>(nodes_.size());

    MeshNode node;

    node.id = id;
    node.x = x;
    node.y = y;
    node.z = z;

    nodes_.push_back(node);

    return id;
}

/**
 * @brief 添加带属性节点
 *
 * @param x X 坐标
 * @param y Y 坐标
 * @param z Z 坐标
 * @param physical_id 物理分组编号
 * @param boundary_id 边界编号
 * @param geometry_id 几何实体编号
 *
 * @return 新节点 ID
 */
int Mesh::add_node(
    double x,
    double y,
    double z,
    int physical_id,
    int boundary_id,
    int geometry_id
)
{
    const int id =
        static_cast<int>(nodes_.size());

    MeshNode node;

    node.id = id;
    node.x = x;
    node.y = y;
    node.z = z;
    node.physical_id = physical_id;
    node.boundary_id = boundary_id;
    node.geometry_id = geometry_id;

    nodes_.push_back(node);

    return id;
}

/**
 * @brief 添加普通单元
 *
 * 单元阶次会根据 CellTopology::order(type) 自动设置。
 *
 * @param type 单元类型
 * @param node_ids 单元节点编号
 *
 * @return 新单元 ID
 */
int Mesh::add_cell(
    CellType type,
    const std::vector<int>& node_ids
)
{
    const int id =
        static_cast<int>(cells_.size());

    MeshCell cell;

    cell.id = id;
    cell.type = type;
    cell.node_ids = node_ids;
    cell.order = CellTopology::order(type);

    cells_.push_back(cell);

    if (cell.order > 1)
    {
        info_.high_order = true;
    }

    return id;
}

/**
 * @brief 添加带属性单元
 *
 * 单元阶次会根据 CellTopology::order(type) 自动设置。
 *
 * @param type 单元类型
 * @param node_ids 单元节点编号
 * @param physical_id 物理分组编号
 * @param material_id 材料编号
 * @param region_id 区域编号
 * @param geometry_id 几何实体编号
 *
 * @return 新单元 ID
 */
int Mesh::add_cell(
    CellType type,
    const std::vector<int>& node_ids,
    int physical_id,
    int material_id,
    int region_id,
    int geometry_id
)
{
    const int id =
        static_cast<int>(cells_.size());

    MeshCell cell;

    cell.id = id;
    cell.type = type;
    cell.node_ids = node_ids;
    cell.order = CellTopology::order(type);
    cell.physical_id = physical_id;
    cell.material_id = material_id;
    cell.region_id = region_id;
    cell.geometry_id = geometry_id;

    cells_.push_back(cell);

    if (cell.order > 1)
    {
        info_.high_order = true;
    }

    return id;
}

/**
 * @brief 判断节点 ID 是否有效
 */
bool Mesh::valid_node_id(
    int node_id
) const
{
    return node_id >= 0 &&
           static_cast<std::size_t>(node_id) < nodes_.size();
}

/**
 * @brief 判断单元 ID 是否有效
 */
bool Mesh::valid_cell_id(
    int cell_id
) const
{
    return cell_id >= 0 &&
           static_cast<std::size_t>(cell_id) < cells_.size();
}

/**
 * @brief 获取节点数量
 */
std::size_t Mesh::num_nodes() const
{
    return nodes_.size();
}

/**
 * @brief 获取单元数量
 */
std::size_t Mesh::num_cells() const
{
    return cells_.size();
}

/**
 * @brief 判断网格是否为空
 */
bool Mesh::empty() const
{
    return nodes_.empty() && cells_.empty();
}

/**
 * @brief 基础网格合法性检查
 *
 * 检查内容：
 *
 * - 节点 ID 是否连续
 * - 单元 ID 是否连续
 * - 单元类型是否有效
 * - 单元节点数量是否符合 CellType
 * - 单元节点编号是否有效
 * - 单元内是否存在重复节点
 *
 * @param error_message 错误信息输出，可为空
 * @return 合法返回 true，否则返回 false
 */
bool Mesh::validate(
    std::string* error_message
) const
{
    for (std::size_t i = 0; i < nodes_.size(); ++i)
    {
        if (nodes_[i].id != static_cast<int>(i))
        {
            if (error_message)
            {
                *error_message =
                    "Mesh::validate: non-contiguous node ids.";
            }

            return false;
        }
    }

    for (std::size_t i = 0; i < cells_.size(); ++i)
    {
        const MeshCell& cell =
            cells_[i];

        if (cell.id != static_cast<int>(i))
        {
            if (error_message)
            {
                *error_message =
                    "Mesh::validate: non-contiguous cell ids.";
            }

            return false;
        }

        if (cell.type == CellType::Unknown)
        {
            if (error_message)
            {
                *error_message =
                    "Mesh::validate: unknown cell type.";
            }

            return false;
        }

        const int expected =
            CellTopology::num_nodes(cell.type);

        if (expected > 0 &&
            static_cast<int>(cell.node_ids.size()) != expected)
        {
            if (error_message)
            {
                *error_message =
                    "Mesh::validate: cell node count mismatch.";
            }

            return false;
        }

        std::set<int> unique_node_ids;

        for (int node_id : cell.node_ids)
        {
            if (!valid_node_id(node_id))
            {
                if (error_message)
                {
                    *error_message =
                        "Mesh::validate: invalid node id in cell.";
                }

                return false;
            }

            unique_node_ids.insert(node_id);
        }

        if (unique_node_ids.size() != cell.node_ids.size())
        {
            if (error_message)
            {
                *error_message =
                    "Mesh::validate: duplicated node id in cell.";
            }

            return false;
        }
    }

    if (error_message)
    {
        error_message->clear();
    }

    return true;
}

/**
 * @brief 计算网格包围盒
 *
 * 返回数组：
 *
 * {
 *   xmin,
 *   xmax,
 *   ymin,
 *   ymax,
 *   zmin,
 *   zmax
 * }
 */
std::array<double, 6> Mesh::bounds() const
{
    if (nodes_.empty())
    {
        return {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }

    double xmin =
        std::numeric_limits<double>::max();

    double ymin =
        std::numeric_limits<double>::max();

    double zmin =
        std::numeric_limits<double>::max();

    double xmax =
        std::numeric_limits<double>::lowest();

    double ymax =
        std::numeric_limits<double>::lowest();

    double zmax =
        std::numeric_limits<double>::lowest();

    for (const auto& node : nodes_)
    {
        xmin = std::min(xmin, node.x);
        xmax = std::max(xmax, node.x);

        ymin = std::min(ymin, node.y);
        ymax = std::max(ymax, node.y);

        zmin = std::min(zmin, node.z);
        zmax = std::max(zmax, node.z);
    }

    return {xmin, xmax, ymin, ymax, zmin, zmax};
}

/**
 * @brief 深复制网格
 *
 * 默认返回 Mesh 基类对象。
 *
 * 派生类如 TriangleMesh、QuadMesh 可以根据需要重写该函数，
 * 返回对应派生类型。
 */
std::shared_ptr<Mesh> Mesh::clone() const
{
    auto result =
        std::make_shared<Mesh>();

    result->info_ = info_;
    result->nodes_ = nodes_;
    result->cells_ = cells_;

    return result;
}

/**
 * @brief 获取网格最高阶次
 *
 * 如果网格为空，返回 0。
 */
int Mesh::order() const
{
    int max_order = 0;

    for (const auto& cell : cells_)
    {
        max_order =
            std::max(max_order, cell.order);
    }

    return max_order;
}

/**
 * @brief 判断是否为高阶网格
 */
bool Mesh::is_high_order() const
{
    return order() > 1;
}

/**
 * @brief 获取网格中出现过的单元类型集合
 */
std::vector<CellType> Mesh::cell_types() const
{
    std::set<CellType> type_set;

    for (const auto& cell : cells_)
    {
        type_set.insert(cell.type);
    }

    return std::vector<CellType>(
        type_set.begin(),
        type_set.end()
    );
}

/**
 * @brief 获取网格维度
 */
MeshDimension Mesh::dimension() const
{
    return info_.dimension;
}

} // namespace OpenCAX
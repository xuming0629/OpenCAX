#pragma once

/**
 * @file MeshPlotter.h
 * @brief OpenCAX 网格绘图工具类声明
 *
 * MeshPlotter 属于 OpenCAX Post/Plot 模块。
 *
 * 该类通过 C++ 调用 Python matplotlib 脚本，实现类似 FEALPy / iFEM 的网格显示能力。
 *
 * 支持：
 *
 * - show_mesh  : 显示完整网格
 * - find_node  : 查找并高亮指定节点
 * - find_edge  : 查找并高亮指定边
 * - find_face  : 查找并高亮指定面
 * - find_cell  : 查找并高亮指定单元
 *
 * 支持网格类型：
 *
 * - 2D 三角形网格
 * - 2D 四边形网格
 * - 3D 四面体网格
 * - 3D 六面体网格
 *
 * 后续可扩展：
 *
 * - Prism6 / Prism15
 * - Pyramid5 / Pyramid13
 * - 混合网格
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/Mesh.h>
#include <OpenCAX/Mesh/MeshTopology.h>

#include <string>

namespace OpenCAX
{

/**
 * @brief 网格绘图模式
 */
enum class MeshPlotMode
{
    /**
     * @brief 显示完整网格
     */
    ShowMesh,

    /**
     * @brief 查找并高亮节点
     */
    FindNode,

    /**
     * @brief 查找并高亮边
     */
    FindEdge,

    /**
     * @brief 查找并高亮面
     */
    FindFace,

    /**
     * @brief 查找并高亮单元
     */
    FindCell
};

/**
 * @brief 网格绘图选项
 */
struct MeshPlotOptions
{
    /**
     * @brief Python 可执行程序
     */
    std::string python_executable = "python3";

    /**
     * @brief Python 绘图脚本路径
     */
    std::string script_path = "tools/open_cax_mesh_plot.py";

    /**
     * @brief 输出图片路径
     *
     * 如果为空，则不保存图片。
     */
    std::string output_path;

    /**
     * @brief 是否弹窗显示
     *
     * 服务器无 GUI 时建议设为 false，并指定 output_path。
     */
    bool show = true;

    /**
     * @brief 是否显示节点编号
     */
    bool show_node_id = false;

    /**
     * @brief 是否显示边编号
     */
    bool show_edge_id = false;

    /**
     * @brief 是否显示面编号
     */
    bool show_face_id = false;

    /**
     * @brief 是否显示单元编号
     */
    bool show_cell_id = true;

    /**
     * @brief 是否显示节点
     */
    bool show_nodes = true;

    /**
     * @brief 是否显示边
     */
    bool show_edges = true;

    /**
     * @brief 是否显示面
     *
     * 对 2D 网格表示显示单元面片。
     * 对 3D 网格表示显示边界面。
     */
    bool show_faces = true;

    /**
     * @brief 是否显示 3D 网格线框
     */
    bool show_wireframe = true;

    /**
     * @brief 是否只显示 3D 边界面
     *
     * 对 Tetra / Hexa 等体网格，通常只显示边界面。
     */
    bool show_boundary_only = true;

    /**
     * @brief 图标题
     */
    std::string title = "OpenCAX Mesh";

    /**
     * @brief 临时 JSON 文件路径
     *
     * 如果为空，则自动生成临时文件。
     */
    std::string json_path;
};

/**
 * @brief 网格绘图工具类
 */
class OpenCAX_API MeshPlotter
{
public:
    /**
     * @brief 显示完整网格
     *
     * @param mesh 输入网格
     * @param topology 网格拓扑
     * @param options 绘图选项
     * @return 成功返回 true，否则返回 false
     */
    static bool show_mesh(
        const Mesh& mesh,
        const MeshTopology& topology,
        const MeshPlotOptions& options = MeshPlotOptions{}
    );

    /**
     * @brief 查找并高亮节点
     *
     * @param mesh 输入网格
     * @param topology 网格拓扑
     * @param node_id 节点 ID
     * @param options 绘图选项
     * @return 成功返回 true，否则返回 false
     */
    static bool find_node(
        const Mesh& mesh,
        const MeshTopology& topology,
        int node_id,
        const MeshPlotOptions& options = MeshPlotOptions{}
    );

    /**
     * @brief 查找并高亮边
     *
     * @param mesh 输入网格
     * @param topology 网格拓扑
     * @param edge_id 边 ID
     * @param options 绘图选项
     * @return 成功返回 true，否则返回 false
     */
    static bool find_edge(
        const Mesh& mesh,
        const MeshTopology& topology,
        int edge_id,
        const MeshPlotOptions& options = MeshPlotOptions{}
    );

    /**
     * @brief 查找并高亮面
     *
     * @param mesh 输入网格
     * @param topology 网格拓扑
     * @param face_id 面 ID
     * @param options 绘图选项
     * @return 成功返回 true，否则返回 false
     */
    static bool find_face(
        const Mesh& mesh,
        const MeshTopology& topology,
        int face_id,
        const MeshPlotOptions& options = MeshPlotOptions{}
    );

    /**
     * @brief 查找并高亮单元
     *
     * @param mesh 输入网格
     * @param topology 网格拓扑
     * @param cell_id 单元 ID
     * @param options 绘图选项
     * @return 成功返回 true，否则返回 false
     */
    static bool find_cell(
        const Mesh& mesh,
        const MeshTopology& topology,
        int cell_id,
        const MeshPlotOptions& options = MeshPlotOptions{}
    );

private:
    static bool run_plot(
        const Mesh& mesh,
        const MeshTopology& topology,
        MeshPlotMode mode,
        int target_id,
        const MeshPlotOptions& options
    );

    static bool write_json(
        const Mesh& mesh,
        const MeshTopology& topology,
        MeshPlotMode mode,
        int target_id,
        const MeshPlotOptions& options,
        const std::string& json_path
    );

    static std::string mode_to_string(
        MeshPlotMode mode
    );

    static std::string json_escape(
        const std::string& text
    );

    static std::string quote_path(
        const std::string& path
    );

    static std::string make_temp_json_path();
};

} // namespace OpenCAX
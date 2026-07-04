#pragma once

#include <string>

namespace OpenCAX
{

struct Color
{
    double r = 1.0;
    double g = 1.0;
    double b = 1.0;

    Color() = default;

    Color(double rr, double gg, double bb)
        : r(rr), g(gg), b(bb)
    {
    }
};

struct VisualOptions
{
    // =========================
    // 显示开关
    // =========================
    bool wireframe = false;

    bool show_nodes = true;
    bool show_edges = true;

    bool show_node_ids = false;
    bool show_cell_ids = false;
    bool show_edge_ids = false;

    bool show_axes = true;

    // =========================
    // 颜色
    // =========================
    Color mesh_color = Color(0.85, 0.85, 0.85);
    Color edge_color = Color(0.05, 0.05, 0.05);
    Color node_color = Color(1.0, 0.2, 0.2);

    Color node_label_color = Color(1.0, 1.0, 0.0);
    Color cell_label_color = Color(0.2, 1.0, 0.2);
    Color edge_label_color = Color(0.2, 0.8, 1.0);

    Color background_color = Color(0.1, 0.1, 0.12);

    // =========================
    // 样式
    // =========================
    double line_width = 2.0;
    double point_size = 8.0;
    double edge_width = 2.0;

    int node_label_font_size = 14;
    int cell_label_font_size = 16;
    int edge_label_font_size = 14;

    // =========================
    // 窗口
    // =========================
    int window_width = 1000;
    int window_height = 800;

    std::string window_title = "OpenCAX Viewer";
};

} // namespace OpenCAX
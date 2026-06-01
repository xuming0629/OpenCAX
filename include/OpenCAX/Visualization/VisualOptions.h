#pragma once

namespace OpenCAX
{

struct Color
{
    double r = 1.0;
    double g = 1.0;
    double b = 1.0;
};

struct VisualOptions
{
    bool show_edges = true;
    bool show_points = false;
    bool show_node_ids = false;
    bool show_cell_ids = false;

    double point_size = 5.0;
    double line_width = 1.5;

    Color mesh_color{0.8, 0.8, 0.85};
    Color edge_color{0.0, 0.0, 0.0};
    Color point_color{1.0, 0.2, 0.2};
    Color node_label_color{1.0, 1.0, 0.0};
    Color cell_label_color{0.0, 1.0, 0.0};

    bool wireframe = true;
};

}
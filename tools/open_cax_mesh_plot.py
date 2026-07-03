#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@file open_cax_mesh_plot.py
@brief OpenCAX 网格绘图脚本

该脚本由 C++ Post/Plot::MeshPlotter 调用。

当前支持：

1. show_mesh
2. find_node
3. find_edge
4. find_cell

当前重点支持 2D 网格：

- Triangle3 / Triangle6
- Quad4 / Quad8 / Quad9

后续可以继续扩展 3D：

- Tetra4 / Tetra10
- Hexa8 / Hexa20 / Hexa27
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Dict, List, Tuple

import matplotlib

# 服务器无 GUI 时也可以保存图片
matplotlib.use("Agg")

import matplotlib.pyplot as plt
from matplotlib.patches import Polygon


CELL_DIMENSION = {
    "Line2": 1,
    "Line3": 1,
    "Triangle3": 2,
    "Triangle6": 2,
    "Quad4": 2,
    "Quad8": 2,
    "Quad9": 2,
    "Tetra4": 3,
    "Tetra10": 3,
    "Hexa8": 3,
    "Hexa20": 3,
    "Hexa27": 3,
}


CELL_CORNER_COUNT = {
    "Line2": 2,
    "Line3": 2,
    "Triangle3": 3,
    "Triangle6": 3,
    "Quad4": 4,
    "Quad8": 4,
    "Quad9": 4,
    "Tetra4": 4,
    "Tetra10": 4,
    "Hexa8": 8,
    "Hexa20": 8,
    "Hexa27": 8,
}


def load_json(path: str) -> Dict:
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def node_map(data: Dict) -> Dict[int, Dict]:
    return {int(n["id"]): n for n in data.get("nodes", [])}


def edge_map(data: Dict) -> Dict[int, Dict]:
    return {int(e["id"]): e for e in data.get("edges", [])}


def cell_map(data: Dict) -> Dict[int, Dict]:
    return {int(c["id"]): c for c in data.get("cells", [])}


def get_xy(nodes: Dict[int, Dict], node_id: int) -> Tuple[float, float]:
    n = nodes[node_id]
    return float(n["x"]), float(n["y"])


def corner_node_ids(cell: Dict) -> List[int]:
    ctype = cell.get("type", "")
    node_ids = [int(i) for i in cell.get("node_ids", [])]
    count = CELL_CORNER_COUNT.get(ctype, len(node_ids))
    return node_ids[:count]


def cell_polygon_points(
    nodes: Dict[int, Dict],
    cell: Dict
) -> List[Tuple[float, float]]:
    ctype = cell.get("type", "")

    if CELL_DIMENSION.get(ctype, 0) != 2:
        return []

    return [get_xy(nodes, nid) for nid in corner_node_ids(cell)]


def cell_center(
    nodes: Dict[int, Dict],
    cell: Dict
) -> Tuple[float, float]:
    pts = cell_polygon_points(nodes, cell)

    if not pts:
        return 0.0, 0.0

    x = sum(p[0] for p in pts) / len(pts)
    y = sum(p[1] for p in pts) / len(pts)

    return x, y


def edge_center(
    nodes: Dict[int, Dict],
    edge: Dict
) -> Tuple[float, float]:
    node_ids = edge.get("node_ids", [])

    if len(node_ids) < 2:
        return 0.0, 0.0

    x0, y0 = get_xy(nodes, int(node_ids[0]))
    x1, y1 = get_xy(nodes, int(node_ids[1]))

    return 0.5 * (x0 + x1), 0.5 * (y0 + y1)


def draw_cells(
    ax,
    data: Dict,
    nodes: Dict[int, Dict],
    options: Dict
):
    if not options.get("show_faces", True):
        return

    for cell in data.get("cells", []):
        pts = cell_polygon_points(nodes, cell)

        if len(pts) < 3:
            continue

        poly = Polygon(
            pts,
            closed=True,
            fill=False,
            edgecolor="black",
            linewidth=0.9,
            zorder=2,
        )
        ax.add_patch(poly)

        if options.get("show_cell_id", False):
            cx, cy = cell_center(nodes, cell)
            ax.text(
                cx,
                cy,
                str(cell["id"]),
                fontsize=10,
                color="tab:blue",
                ha="center",
                va="center",
                zorder=10,
            )


def draw_edges(
    ax,
    data: Dict,
    nodes: Dict[int, Dict],
    options: Dict
):
    if not options.get("show_edges", True):
        return

    for edge in data.get("edges", []):
        node_ids = edge.get("node_ids", [])

        if len(node_ids) < 2:
            continue

        a = int(node_ids[0])
        b = int(node_ids[1])

        if a not in nodes or b not in nodes:
            continue

        x0, y0 = get_xy(nodes, a)
        x1, y1 = get_xy(nodes, b)

        ax.plot(
            [x0, x1],
            [y0, y1],
            color="black",
            linewidth=0.8,
            zorder=3,
        )

        if options.get("show_edge_id", False):
            cx, cy = edge_center(nodes, edge)
            ax.text(
                cx,
                cy,
                str(edge["id"]),
                fontsize=8,
                color="tab:green",
                ha="center",
                va="center",
                zorder=10,
            )


def draw_nodes(
    ax,
    data: Dict,
    options: Dict
):
    if not options.get("show_nodes", True):
        return

    xs = []
    ys = []

    for node in data.get("nodes", []):
        xs.append(float(node["x"]))
        ys.append(float(node["y"]))

    ax.scatter(
        xs,
        ys,
        s=22,
        color="black",
        zorder=8,
    )

    if options.get("show_node_id", False):
        for node in data.get("nodes", []):
            ax.text(
                float(node["x"]),
                float(node["y"]),
                str(node["id"]),
                fontsize=9,
                color="tab:red",
                ha="left",
                va="bottom",
                zorder=12,
            )


def highlight_node(
    ax,
    nodes: Dict[int, Dict],
    node_id: int
):
    if node_id not in nodes:
        return

    x, y = get_xy(nodes, node_id)

    ax.scatter(
        [x],
        [y],
        s=150,
        color="red",
        zorder=30,
    )

    ax.text(
        x,
        y,
        f" node {node_id}",
        fontsize=12,
        color="red",
        ha="left",
        va="bottom",
        zorder=31,
    )


def highlight_edge(
    ax,
    edges: Dict[int, Dict],
    nodes: Dict[int, Dict],
    edge_id: int
):
    if edge_id not in edges:
        return

    edge = edges[edge_id]
    node_ids = edge.get("node_ids", [])

    if len(node_ids) < 2:
        return

    a = int(node_ids[0])
    b = int(node_ids[1])

    if a not in nodes or b not in nodes:
        return

    x0, y0 = get_xy(nodes, a)
    x1, y1 = get_xy(nodes, b)

    ax.plot(
        [x0, x1],
        [y0, y1],
        color="red",
        linewidth=3.0,
        zorder=30,
    )

    ax.scatter(
        [x0, x1],
        [y0, y1],
        s=80,
        color="red",
        zorder=31,
    )

    ax.text(
        0.5 * (x0 + x1),
        0.5 * (y0 + y1),
        f" edge {edge_id}",
        fontsize=12,
        color="red",
        ha="center",
        va="center",
        zorder=32,
    )


def highlight_cell(
    ax,
    cells: Dict[int, Dict],
    nodes: Dict[int, Dict],
    cell_id: int
):
    if cell_id not in cells:
        return

    cell = cells[cell_id]
    pts = cell_polygon_points(nodes, cell)

    if len(pts) < 3:
        return

    poly = Polygon(
        pts,
        closed=True,
        fill=True,
        facecolor="red",
        edgecolor="red",
        alpha=0.35,
        linewidth=2.5,
        zorder=25,
    )
    ax.add_patch(poly)

    cx, cy = cell_center(nodes, cell)

    ax.text(
        cx,
        cy,
        f"cell {cell_id}",
        fontsize=12,
        color="red",
        ha="center",
        va="center",
        zorder=31,
    )


def setup_axes(
    ax,
    data: Dict,
    options: Dict
):
    xs = [float(n["x"]) for n in data.get("nodes", [])]
    ys = [float(n["y"]) for n in data.get("nodes", [])]

    if not xs or not ys:
        return

    xmin = min(xs)
    xmax = max(xs)
    ymin = min(ys)
    ymax = max(ys)

    dx = xmax - xmin
    dy = ymax - ymin

    if dx <= 0.0:
        dx = 1.0

    if dy <= 0.0:
        dy = 1.0

    pad = 0.08 * max(dx, dy)

    ax.set_xlim(xmin - pad, xmax + pad)
    ax.set_ylim(ymin - pad, ymax + pad)

    ax.set_aspect("equal", adjustable="box")
    ax.grid(True, linestyle="--", linewidth=0.5, alpha=0.45)

    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_title(options.get("title", "OpenCAX Mesh"))


def plot_mesh(data: Dict):
    nodes = node_map(data)
    edges = edge_map(data)
    cells = cell_map(data)

    options = data.get("options", {})
    mode = data.get("mode", "show_mesh")
    target_id = int(data.get("target_id", -1))

    fig, ax = plt.subplots(figsize=(8, 8))

    draw_cells(ax, data, nodes, options)
    draw_edges(ax, data, nodes, options)
    draw_nodes(ax, data, options)

    if mode == "find_node":
        highlight_node(ax, nodes, target_id)
    elif mode == "find_edge":
        highlight_edge(ax, edges, nodes, target_id)
    elif mode == "find_cell":
        highlight_cell(ax, cells, nodes, target_id)

    setup_axes(ax, data, options)

    output_path = options.get("output_path", "")

    if output_path:
        output_path = Path(output_path)
        output_path.parent.mkdir(parents=True, exist_ok=True)
        plt.savefig(output_path, dpi=200, bbox_inches="tight")
        print(f"[OpenCAX Plot] saved to: {output_path}")

    if options.get("show", True):
        # 因为上面用了 Agg，服务器模式下不会弹窗。
        # 如果你想本地弹窗，可以去掉 matplotlib.use("Agg")。
        plt.show()

    plt.close(fig)


def parse_args():
    parser = argparse.ArgumentParser(
        description="OpenCAX mesh plotter"
    )

    parser.add_argument(
        "--input",
        required=True,
        help="Input mesh JSON file"
    )

    return parser.parse_args()


def main():
    args = parse_args()
    data = load_json(args.input)
    plot_mesh(data)


if __name__ == "__main__":
    main()

/**
 * @FileName      : OpenCAX_Types.h
 * @Time          : 2026-06-04 09:18:24
 * @Author        : XuMing
 * @Email         : 920972751@qq.com
 * @Description   : OpenCAX 几何建模基础枚举类型定义
 * @Company       : 2026 XuMing. All Rights Reserved.
 */

#pragma once

namespace OpenCAX
{

/**
 * @brief 几何拓扑形状类型枚举
 */
enum class ShapeType
{
    Unknown,    ///< 未知类型
    Vertex,     ///< 顶点
    Edge,       ///< 边
    Wire,       ///< 轮廓/折线
    Face,       ///< 面
    Shell,      ///< 壳
    Solid,      ///< 实体
    Compound    ///< 组合体
};

/**
 * @brief 几何布尔运算类型枚举
 */
enum class BooleanOperation
{
    Fuse,       ///< 并集（融合）
    Cut,        ///< 差集（切割）
    Common      ///< 交集
};

/**
 * @brief 几何曲线/曲面连续性类型枚举
 */
enum class ContinuityType
{
    C0,         ///< 位置连续
    C1,         ///< 一阶导数连续
    C2,         ///< 二阶导数连续
    G1,         ///< 几何切向连续
    G2          ///< 几何曲率连续
};

} // namespace OpenCAX
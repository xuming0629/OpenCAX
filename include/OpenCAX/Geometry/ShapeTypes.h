#pragma once

namespace OpenCAX
{

/**
 * @brief OpenCAX 几何拓扑类型
 */
enum class ShapeType
{
    Unknown,
    Vertex,
    Edge,
    Wire,
    Face,
    Shell,
    Solid,
    Compound,
    CompoundSolid
};

/**
 * @brief 几何布尔运算类型
 */
enum class BooleanOperation
{
    Fuse,
    Cut,
    Common,
    Section
};

} // namespace OpenCAX

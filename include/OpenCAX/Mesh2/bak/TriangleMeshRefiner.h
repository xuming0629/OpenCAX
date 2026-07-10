#pragma once

/**
 * @file TriangleMeshRefiner.h
 * @brief 三角形网格加密器兼容头文件
 *
 * 本文件用于保持旧版 OpenCAX 代码的兼容性。
 *
 * 旧版本中可能使用：
 *
 * @code
 * #include <OpenCAX/Mesh2/TriangleMeshRefiner.h>
 *
 * OpenCAX::TriangleMeshRefiner::uniform_refine(mesh, levels);
 * OpenCAX::TriangleMeshRefiner::bisection_refine(mesh, levels);
 * @endcode
 *
 * 新版本中，网格加密功能已经迁移到 Meshing 模块：
 *
 * @code
 * #include <OpenCAX/Meshing/Refinement/MeshRefiner.h>
 *
 * OpenCAX::MeshRefiner::uniform_refine(mesh, levels);
 * OpenCAX::MeshRefiner::bisection_refine(mesh, levels);
 * @endcode
 *
 * 因此这里通过 using alias 保持旧接口可用。
 *
 * @note
 * 新代码建议直接使用：
 *
 * @code
 * OpenCAX::MeshRefiner
 * @endcode
 *
 * @note
 * 该文件后续可以在 OpenCAX 1.0 之后考虑废弃。
 */

#include <OpenCAX/Meshing/Refinement/MeshRefiner.h>

namespace OpenCAX
{

/**
 * @brief TriangleMeshRefiner 旧接口兼容别名
 *
 * TriangleMeshRefiner 是 MeshRefiner 的兼容别名。
 *
 * 目的：
 *
 * - 保证旧代码无需立即修改
 * - 平滑迁移到新的 Meshing/Refinement 模块
 *
 * 推荐新代码使用：
 *
 * @code
 * MeshRefiner
 * @endcode
 */
using TriangleMeshRefiner = MeshRefiner;

} // namespace OpenCAX
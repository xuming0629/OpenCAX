#pragma once

/**
 * @file TriangleMeshRefiner.h
 * @brief OpenCAX 三角形网格加密工具
 *
 * 当前支持：
 *
 * - 一致加密 / uniform refinement
 * - 最长边二分加密 / longest-edge bisection refinement
 * - 指定单元局部二分加密
 *
 * 设计说明：
 *
 * 1. TriangleMeshRefiner 不修改原始网格，而是返回一个新 TriangleMesh。
 * 2. 所有新节点使用边中点。
 * 3. 共享边只创建一个中点，保证拓扑一致。
 * 4. 局部二分时，如果邻居共享被切分边，也会同步切分，避免悬挂点。
 */

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

#include <vector>

namespace OpenCAX
{

class OpenCAX_API TriangleMeshRefiner
{
public:
    /**
     * @brief 一致加密
     *
     * 每个三角形一分四。
     *
     * @param mesh 输入三角形网格
     * @param levels 加密层数
     * @return 加密后的新 TriangleMesh
     */
    static TriangleMesh uniform_refine(
        const TriangleMesh& mesh,
        int levels = 1
    );

    /**
     * @brief 二分加密
     *
     * 对所有三角形的最长边做二分。
     *
     * @param mesh 输入三角形网格
     * @param levels 加密层数
     * @return 加密后的新 TriangleMesh
     */
    static TriangleMesh bisection_refine(
        const TriangleMesh& mesh,
        int levels = 1
    );

    /**
     * @brief 对指定单元做最长边二分加密
     *
     * 注意：
     *
     * 如果某个单元的最长边被切分，
     * 共享该边的邻居单元也会被同步切分，
     * 以避免产生悬挂点。
     *
     * @param mesh 输入三角形网格
     * @param marked_cell_ids 需要加密的单元 ID
     * @return 加密后的新 TriangleMesh
     */
    static TriangleMesh bisection_refine_marked_cells(
        const TriangleMesh& mesh,
        const std::vector<int>& marked_cell_ids
    );

private:
    TriangleMeshRefiner() = delete;
};

} // namespace OpenCAX
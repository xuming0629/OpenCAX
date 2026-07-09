#pragma once

#include <OpenCAX/Core/Config.h>
#include <OpenCAX/Mesh/TriangleMesh.h>

namespace OpenCAX
{

class OpenCAX_API TriangleMeshOrderConverter
{
public:
    /**
     * @brief 将一阶 Triangle3 网格转换为二阶 Triangle6 网格
     *
     * Triangle3:
     *
     *     [v0, v1, v2]
     *
     * Triangle6:
     *
     *     [v0, v1, v2, m01, m12, m20]
     *
     * 其中：
     *
     *     m01 是 edge(v0, v1) 中点
     *     m12 是 edge(v1, v2) 中点
     *     m20 是 edge(v2, v0) 中点
     */
    static TriangleMesh to_quadratic(
        const TriangleMesh& mesh
    );
};

} // namespace OpenCAX
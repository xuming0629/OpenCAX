#pragma once

#include <OpenCAX/Mesh/Mesh.h>
#include <OpenCAX/Post/VisualOptions.h>

namespace OpenCAX
{

class VtkViewer
{
public:
    static void showMesh(const Mesh& mesh);

    static void showMesh(
        const Mesh& mesh,
        const VisualOptions& options
    );

    static void showMeshWithNodeIds(const Mesh& mesh);
    static void showMeshWithCellIds(const Mesh& mesh);
    static void showMeshWithIds(const Mesh& mesh);
};

}
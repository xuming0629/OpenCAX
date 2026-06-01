#pragma once

#include <OpenCAX/Meshing/Mesh.h>
#include <OpenCAX/Visualization/VisualOptions.h>

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
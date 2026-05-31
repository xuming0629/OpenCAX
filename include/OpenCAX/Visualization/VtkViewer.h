#pragma once

#include <OpenCAX/Meshing/Mesh.h>

namespace OpenCAX
{

class VtkViewer
{
public:
    static void showMesh(const Mesh& mesh);
};

}
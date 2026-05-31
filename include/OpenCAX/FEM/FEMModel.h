#pragma once

#include <OpenCAX/Meshing/Mesh.h>

namespace OpenCAX
{

class FEMModel
{
public:
    void setMesh(const Mesh& mesh);

private:
    Mesh mesh_;
};

}
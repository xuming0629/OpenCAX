#pragma once

#include <OpenCAX/Mesh/Mesh.h>

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
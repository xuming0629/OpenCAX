#include <OpenCAX/FEM/FEMModel.h>

namespace OpenCAX
{

void FEMModel::setMesh(const Mesh& mesh)
{
    mesh_ = mesh;
}

}
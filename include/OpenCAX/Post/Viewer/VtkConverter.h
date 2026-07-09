#pragma once

#include <OpenCAX/Mesh2/Mesh.h>

#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

namespace OpenCAX
{

class VtkConverter
{
public:
    static vtkSmartPointer<vtkUnstructuredGrid> meshToGrid(
        const Mesh& mesh
    );

private:
    static int toVtkCellType(CellType type);
};

}
#include <OpenCAX/Post/Viewer/VtkConverter.h>

#include <vtkCellType.h>
#include <vtkIdList.h>
#include <vtkNew.h>
#include <vtkPoints.h>

namespace OpenCAX
{

int VtkConverter::toVtkCellType(CellType type)
{
    switch (type) {
    case CellType::Line2:
        return VTK_LINE;
    case CellType::Triangle3:
        return VTK_TRIANGLE;
    case CellType::Quad4:
        return VTK_QUAD;
    case CellType::Tetra4:
        return VTK_TETRA;
    case CellType::Hexa8:
        return VTK_HEXAHEDRON;
    default:
        return VTK_EMPTY_CELL;
    }
}

vtkSmartPointer<vtkUnstructuredGrid> VtkConverter::meshToGrid(
    const Mesh& mesh
)
{
    vtkNew<vtkPoints> points;

    for (const auto& node : mesh.nodes()) {
        points->InsertNextPoint(node.x, node.y, node.z);
    }

    vtkSmartPointer<vtkUnstructuredGrid> grid =
        vtkSmartPointer<vtkUnstructuredGrid>::New();

    grid->SetPoints(points);

    for (const auto& cell : mesh.cells()) {
        int vtk_type = toVtkCellType(cell.type);

        if (vtk_type == VTK_EMPTY_CELL) {
            continue;
        }

        vtkNew<vtkIdList> ids;

        for (int nid : cell.node_ids) {
            ids->InsertNextId(nid);
        }

        grid->InsertNextCell(vtk_type, ids);
    }

    return grid;
}

}
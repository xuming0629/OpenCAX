#include <OpenCAX/Post/Viewer/VtkConverter.h>

#include <vtkCellType.h>
#include <vtkIdList.h>
#include <vtkNew.h>
#include <vtkPoints.h>

#include <iostream>
#include <stdexcept>

namespace OpenCAX
{

int VtkConverter::toVtkCellType(CellType type)
{
    switch (type)
    {
    case CellType::Line2:
        return VTK_LINE;

    case CellType::Triangle3:
        return VTK_TRIANGLE;

    case CellType::Triangle6:
        return VTK_QUADRATIC_TRIANGLE;

    case CellType::Quad4:
        return VTK_QUAD;

    case CellType::Quad8:
        return VTK_QUADRATIC_QUAD;

    case CellType::Quad9:
        return VTK_BIQUADRATIC_QUAD;

    case CellType::Tetra4:
        return VTK_TETRA;

    case CellType::Tetra10:
        return VTK_QUADRATIC_TETRA;

    case CellType::Hexa8:
        return VTK_HEXAHEDRON;

    case CellType::Hexa20:
        return VTK_QUADRATIC_HEXAHEDRON;

    case CellType::Hexa27:
        return VTK_TRIQUADRATIC_HEXAHEDRON;

    default:
        return VTK_EMPTY_CELL;
    }
}

vtkSmartPointer<vtkUnstructuredGrid>
VtkConverter::meshToGrid(const Mesh& mesh)
{
    vtkNew<vtkPoints> points;

    for (const auto& node : mesh.nodes())
    {
        points->InsertNextPoint(
            node.x,
            node.y,
            node.z
        );
    }

    vtkSmartPointer<vtkUnstructuredGrid> grid =
        vtkSmartPointer<vtkUnstructuredGrid>::New();

    grid->SetPoints(points);

    for (const auto& cell : mesh.cells())
    {
        const int vtk_type =
            toVtkCellType(cell.type);

        if (vtk_type == VTK_EMPTY_CELL)
        {
            std::cerr
                << "[VtkConverter] unsupported cell type: "
                << static_cast<int>(cell.type)
                << "\n";

            continue;
        }

        vtkNew<vtkIdList> ids;

        for (const auto nid : cell.node_ids)
        {
            if (nid < 0 ||
                static_cast<std::size_t>(nid) >= mesh.num_nodes())
            {
                throw std::runtime_error(
                    "VtkConverter: invalid node id in cell"
                );
            }

            ids->InsertNextId(
                static_cast<vtkIdType>(nid)
            );
        }

        grid->InsertNextCell(
            vtk_type,
            ids
        );
    }

    return grid;
}

} // namespace OpenCAX
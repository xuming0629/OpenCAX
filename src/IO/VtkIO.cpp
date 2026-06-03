#include <OpenCAX/IO/VtkIO.h>
#include <OpenCAX/Core/LogMacros.h>

#include <vtkCellType.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>

namespace OpenCAX
{

namespace
{

int toVtkCellType(CellType type)
{
    switch (type) {
    case CellType::Point1:
        return VTK_VERTEX;

    case CellType::Line2:
        return VTK_LINE;

    case CellType::Line3:
        return VTK_QUADRATIC_EDGE;

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

    case CellType::Polygon:
        return VTK_POLYGON;

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

    case CellType::Prism6:
        return VTK_WEDGE;

    case CellType::Pyramid5:
        return VTK_PYRAMID;

    default:
        return VTK_EMPTY_CELL;
    }
}

bool checkCellNodeCount(CellType type, std::size_t n)
{
    switch (type) {
    case CellType::Point1:
        return n == 1;

    case CellType::Line2:
        return n == 2;

    case CellType::Line3:
        return n == 3;

    case CellType::Triangle3:
        return n == 3;

    case CellType::Triangle6:
        return n == 6;

    case CellType::Quad4:
        return n == 4;

    case CellType::Quad8:
        return n == 8;

    case CellType::Quad9:
        return n == 9;

    case CellType::Polygon:
        return n >= 3;

    case CellType::Tetra4:
        return n == 4;

    case CellType::Tetra10:
        return n == 10;

    case CellType::Hexa8:
        return n == 8;

    case CellType::Hexa20:
        return n == 20;

    case CellType::Hexa27:
        return n == 27;

    case CellType::Prism6:
        return n == 6;

    case CellType::Pyramid5:
        return n == 5;

    default:
        return false;
    }
}

} // namespace

bool VtkIO::writeVTU(
    const std::vector<MeshNode>& nodes,
    const std::vector<std::vector<int>>& cells,
    const std::vector<CellType>& cell_types,
    const std::string& filename,
    bool binary
)
{
    if (nodes.empty()) {
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to write VTU: nodes is empty."
        );
        return false;
    }

    if (cells.empty()) {
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to write VTU: cells is empty."
        );
        return false;
    }

    if (cells.size() != cell_types.size()) {
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to write VTU: cells size {} != cell_types size {}.",
            cells.size(),
            cell_types.size()
        );
        return false;
    }

    vtkNew<vtkPoints> points;
    points->SetNumberOfPoints(static_cast<vtkIdType>(nodes.size()));

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        const auto& p = nodes[i];

        points->SetPoint(
            static_cast<vtkIdType>(i),
            p.x,
            p.y,
            p.z
        );
    }

    vtkNew<vtkUnstructuredGrid> grid;
    grid->SetPoints(points);

    std::size_t skipped_cells = 0;

    for (std::size_t i = 0; i < cells.size(); ++i) {
        const auto& ids = cells[i];
        const CellType type = cell_types[i];

        const int vtk_type = toVtkCellType(type);

        if (vtk_type == VTK_EMPTY_CELL) {
            ++skipped_cells;
            continue;
        }

        if (!checkCellNodeCount(type, ids.size())) {
            ++skipped_cells;
            continue;
        }

        vtkNew<vtkIdList> id_list;
        id_list->SetNumberOfIds(static_cast<vtkIdType>(ids.size()));

        bool valid_ids = true;

        for (std::size_t j = 0; j < ids.size(); ++j) {
            const int id = ids[j];

            if (id < 0 || static_cast<std::size_t>(id) >= nodes.size()) {
                valid_ids = false;
                break;
            }

            id_list->SetId(
                static_cast<vtkIdType>(j),
                static_cast<vtkIdType>(id)
            );
        }

        if (!valid_ids) {
            ++skipped_cells;
            continue;
        }

        grid->InsertNextCell(vtk_type, id_list);
    }

    if (skipped_cells > 0) {
        OpenCAX_LOG_WARN(
            LogModule::IO,
            "Skipped {} unsupported or invalid cells when writing VTU.",
            skipped_cells
        );
    }

    vtkNew<vtkXMLUnstructuredGridWriter> writer;
    writer->SetFileName(filename.c_str());
    writer->SetInputData(grid);

    if (binary) {
        writer->SetDataModeToBinary();
    }
    else {
        writer->SetDataModeToAscii();
    }

    const int ok = writer->Write();

    if (!ok) {
        OpenCAX_LOG_ERROR(
            LogModule::IO,
            "Failed to write VTU file: {}",
            filename
        );
        return false;
    }

    OpenCAX_LOG_INFO(
        LogModule::IO,
        "VTU mesh written: {}",
        filename
    );

    OpenCAX_LOG_INFO(
        LogModule::IO,
        "Mesh stats -> nodes: {}, cells: {}",
        nodes.size(),
        cells.size()
    );

    return true;
}

} // namespace OpenCAX
#include <OpenCAX/Post/ScalarFieldViewer.h>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTriangle.h>
#include <vtkUnstructuredGrid.h>

#include <iostream>
#include <string>

namespace OpenCAX
{

void ScalarFieldViewer::showSolution(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& values,
    const char* title
)
{
    showSolution2D(
        mesh,
        values,
        title
    );
}

void ScalarFieldViewer::showSolution2D(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& values,
    const char* title
)
{
    if (values.size() != static_cast<int>(mesh.num_nodes()))
    {
        std::cerr << "[OpenCAX::ScalarFieldViewer] scalar size mismatch. "
                  << "values.size() = "
                  << values.size()
                  << ", mesh.num_nodes() = "
                  << mesh.num_nodes()
                  << std::endl;
        return;
    }

    vtkNew<vtkPoints> points;

    const auto& nodes = mesh.nodes();

    for (const auto& node : nodes)
    {
        points->InsertNextPoint(
            node.x,
            node.y,
            node.z
        );
    }

    vtkNew<vtkUnstructuredGrid> grid;
    grid->SetPoints(points);

    const auto& cells = mesh.cells();

    for (const auto& cell : cells)
    {
        if (cell.type != CellType::Triangle3)
        {
            continue;
        }

        if (cell.node_ids.size() != 3)
        {
            continue;
        }

        vtkNew<vtkTriangle> tri;

        tri->GetPointIds()->SetId(
            0,
            cell.node_ids[0]
        );

        tri->GetPointIds()->SetId(
            1,
            cell.node_ids[1]
        );

        tri->GetPointIds()->SetId(
            2,
            cell.node_ids[2]
        );

        grid->InsertNextCell(
            tri->GetCellType(),
            tri->GetPointIds()
        );
    }

    vtkNew<vtkDoubleArray> scalars;
    scalars->SetName("solution");
    scalars->SetNumberOfComponents(1);

    for (int i = 0; i < values.size(); ++i)
    {
        scalars->InsertNextValue(
            values(i)
        );
    }

    grid->GetPointData()->SetScalars(
        scalars
    );

    double range[2];
    scalars->GetRange(range);

    vtkNew<vtkLookupTable> lut;
    lut->SetNumberOfTableValues(256);
    lut->SetHueRange(0.667, 0.0);
    lut->Build();

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(grid);
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(range);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetEdgeVisibility(true);
    actor->GetProperty()->SetLineWidth(1.0);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(1.0, 1.0, 1.0);

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);
    window->SetWindowName(
        title ? title : "Scalar Field"
    );
    window->SetSize(1000, 800);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(window);

    window->Render();
    interactor->Start();
}

} // namespace OpenCAX
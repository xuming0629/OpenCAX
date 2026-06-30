#include <OpenCAX/Post/ScalarFieldViewer.h>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkNamedColors.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTriangle.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>

#include <iostream>

namespace OpenCAX
{

void ScalarFieldViewer::showSolution(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& u,
    const char* title)
{
    if (static_cast<std::size_t>(u.size()) != mesh.num_nodes())
    {
        std::cerr << "[ScalarFieldViewer] solution size mismatch." << std::endl;
        return;
    }

    auto points = vtkSmartPointer<vtkPoints>::New();

    for (std::size_t i = 0; i < mesh.num_nodes(); ++i)
    {
        auto p = mesh.point2d(static_cast<int>(i));
        points->InsertNextPoint(p[0], p[1], u[static_cast<int>(i)]);
    }

    auto cells = vtkSmartPointer<vtkCellArray>::New();

    for (std::size_t cid = 0; cid < mesh.num_triangles(); ++cid)
    {
        auto tri = mesh.triangle(cid);

        auto vtk_tri = vtkSmartPointer<vtkTriangle>::New();
        vtk_tri->GetPointIds()->SetId(0, tri[0]);
        vtk_tri->GetPointIds()->SetId(1, tri[1]);
        vtk_tri->GetPointIds()->SetId(2, tri[2]);

        cells->InsertNextCell(vtk_tri);
    }

    auto grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    grid->SetPoints(points);
    grid->SetCells(VTK_TRIANGLE, cells);

    auto scalars = vtkSmartPointer<vtkDoubleArray>::New();
    scalars->SetName("u");

    for (int i = 0; i < u.size(); ++i)
    {
        scalars->InsertNextValue(u[i]);
    }

    grid->GetPointData()->SetScalars(scalars);

    auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(grid);
    mapper->ScalarVisibilityOn();
    mapper->SetScalarModeToUsePointData();
    mapper->SetColorModeToMapScalars();
    mapper->SetScalarRange(u.minCoeff(), u.maxCoeff());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.12);

    auto window = vtkSmartPointer<vtkRenderWindow>::New();
    window->AddRenderer(renderer);
    window->SetWindowName(title);
    window->SetSize(1000, 800);

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(window);

    renderer->ResetCamera();

    window->Render();
    interactor->Start();
}

} // namespace OpenCAX
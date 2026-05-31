#include <OpenCAX/Visualization/VtkViewer.h>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnstructuredGrid.h>

namespace OpenCAX
{

void VtkViewer::showMesh(const Mesh& mesh)
{
    vtkNew<vtkPoints> points;

    for (const auto& node : mesh.nodes()) {
        points->InsertNextPoint(node.x, node.y, node.z);
    }

    vtkNew<vtkUnstructuredGrid> grid;
    grid->SetPoints(points);

    for (const auto& cell : mesh.cells()) {
        vtkNew<vtkIdList> ids;

        for (int id : cell.node_ids) {
            ids->InsertNextId(id);
        }

        int vtk_cell_type = 10; // VTK_TETRA by default
        grid->InsertNextCell(vtk_cell_type, ids);
    }

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(grid);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetRepresentationToWireframe();

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->ResetCamera();

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);
    window->SetSize(1000, 800);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(window);

    window->Render();
    interactor->Start();
}

}
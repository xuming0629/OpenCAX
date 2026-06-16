#include <OpenCAX/Post/VtkViewer.h>
#include <OpenCAX/Post/VtkConverter.h>

#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkCellCenters.h>
#include <vtkDataSetMapper.h>
#include <vtkLabeledDataMapper.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStringArray.h>
#include <vtkTextProperty.h>
#include <vtkVertexGlyphFilter.h>

#include <string>

namespace OpenCAX
{

static void add_node_labels(
    vtkRenderer* renderer,
    vtkUnstructuredGrid* grid,
    const VisualOptions& options
)
{
    vtkNew<vtkVertexGlyphFilter> glyph;
    glyph->SetInputData(grid);
    glyph->Update();

    vtkNew<vtkLabeledDataMapper> mapper;
    mapper->SetInputConnection(glyph->GetOutputPort());

    // 直接显示点 ID
    mapper->SetLabelModeToLabelIds();

    mapper->GetLabelTextProperty()->SetFontSize(14);
    mapper->GetLabelTextProperty()->SetColor(
        options.node_label_color.r,
        options.node_label_color.g,
        options.node_label_color.b
    );

    vtkNew<vtkActor2D> actor;
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}

static void add_cell_labels(
    vtkRenderer* renderer,
    vtkUnstructuredGrid* grid,
    const VisualOptions& options
)
{
    vtkNew<vtkCellCenters> centers;
    centers->SetInputData(grid);
    centers->Update();

    vtkNew<vtkLabeledDataMapper> mapper;
    mapper->SetInputConnection(centers->GetOutputPort());

    // 显示中心点 ID，等价于 cell id
    mapper->SetLabelModeToLabelIds();

    mapper->GetLabelTextProperty()->SetFontSize(16);
    mapper->GetLabelTextProperty()->SetColor(
        options.cell_label_color.r,
        options.cell_label_color.g,
        options.cell_label_color.b
    );

    vtkNew<vtkActor2D> actor;
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}

void VtkViewer::showMesh(const Mesh& mesh)
{
    VisualOptions options;
    showMesh(mesh, options);
}

void VtkViewer::showMeshWithNodeIds(const Mesh& mesh)
{
    VisualOptions options;
    options.show_node_ids = true;
    showMesh(mesh, options);
}

void VtkViewer::showMeshWithCellIds(const Mesh& mesh)
{
    VisualOptions options;
    options.show_cell_ids = true;
    showMesh(mesh, options);
}

void VtkViewer::showMeshWithIds(const Mesh& mesh)
{
    VisualOptions options;
    options.show_node_ids = true;
    options.show_cell_ids = true;
    showMesh(mesh, options);
}

void VtkViewer::showMesh(
    const Mesh& mesh,
    const VisualOptions& options
)
{
    auto grid = VtkConverter::meshToGrid(mesh);

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(grid);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    if (options.wireframe) {
        actor->GetProperty()->SetRepresentationToWireframe();
    }

    actor->GetProperty()->SetColor(
        options.mesh_color.r,
        options.mesh_color.g,
        options.mesh_color.b
    );

    actor->GetProperty()->SetLineWidth(options.line_width);
    actor->GetProperty()->SetPointSize(options.point_size);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(0.1, 0.1, 0.12);

    if (options.show_node_ids) {
        add_node_labels(renderer, grid, options);
    }

    if (options.show_cell_ids) {
        add_cell_labels(renderer, grid, options);
    }

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);
    window->SetSize(1000, 800);
    window->SetWindowName("OpenCAX Viewer");

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(window);

    renderer->ResetCamera();

    window->Render();
    interactor->Start();
}

}
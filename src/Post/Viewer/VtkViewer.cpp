
#include <OpenCAX/Post/Viewer/VtkViewer.h>
#include <OpenCAX/Post/Viewer/VtkConverter.h>
#include <OpenCAX/Mesh2/MeshTopology.h>

#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkAxesActor.h>
#include <vtkCellArray.h>
#include <vtkCellCenters.h>
#include <vtkDataSetMapper.h>
#include <vtkLabeledDataMapper.h>
#include <vtkLine.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStringArray.h>
#include <vtkTextProperty.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVertexGlyphFilter.h>

#include <string>

namespace OpenCAX
{

namespace
{

static vtkSmartPointer<vtkStringArray> make_id_array(
    vtkIdType count,
    const std::string& name,
    const std::string& prefix = ""
)
{
    vtkSmartPointer<vtkStringArray> labels =
        vtkSmartPointer<vtkStringArray>::New();

    labels->SetName(name.c_str());
    labels->SetNumberOfComponents(1);
    labels->SetNumberOfTuples(count);

    for (vtkIdType i = 0; i < count; ++i)
    {
        labels->SetValue(
            i,
            prefix + std::to_string(i)
        );
    }

    return labels;
}

static vtkSmartPointer<vtkPolyData> build_edge_polydata_from_topology(
    const Mesh& mesh,
    const MeshTopology& topology
)
{
    vtkSmartPointer<vtkPoints> points =
        vtkSmartPointer<vtkPoints>::New();

    const auto& nodes = mesh.nodes();

    for (const auto& node : nodes)
    {
        points->InsertNextPoint(
            node.x,
            node.y,
            node.z
        );
    }

    vtkSmartPointer<vtkCellArray> lines =
        vtkSmartPointer<vtkCellArray>::New();

    const auto& edges = topology.edges();

    for (const auto& edge : edges)
    {
        vtkNew<vtkLine> line;

        line->GetPointIds()->SetId(
            0,
            static_cast<vtkIdType>(edge[0])
        );

        line->GetPointIds()->SetId(
            1,
            static_cast<vtkIdType>(edge[1])
        );

        lines->InsertNextCell(line);
    }

    vtkSmartPointer<vtkPolyData> polydata =
        vtkSmartPointer<vtkPolyData>::New();

    polydata->SetPoints(points);
    polydata->SetLines(lines);

    return polydata;
}

static void add_node_actor(
    vtkRenderer* renderer,
    vtkUnstructuredGrid* grid,
    const VisualOptions& options
)
{
    vtkNew<vtkVertexGlyphFilter> glyph;
    glyph->SetInputData(grid);
    glyph->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(glyph->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    actor->GetProperty()->SetColor(
        options.node_color.r,
        options.node_color.g,
        options.node_color.b
    );

    actor->GetProperty()->SetPointSize(
        options.point_size
    );

    renderer->AddActor(actor);
}

static void add_edge_actor(
    vtkRenderer* renderer,
    const Mesh& mesh,
    const MeshTopology& topology,
    const VisualOptions& options
)
{
    vtkSmartPointer<vtkPolyData> edge_polydata =
        build_edge_polydata_from_topology(
            mesh,
            topology
        );

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(edge_polydata);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    actor->GetProperty()->SetColor(
        options.edge_color.r,
        options.edge_color.g,
        options.edge_color.b
    );

    actor->GetProperty()->SetLineWidth(
        options.edge_width
    );

    renderer->AddActor(actor);
}

static void add_node_labels(
    vtkRenderer* renderer,
    vtkUnstructuredGrid* grid,
    const VisualOptions& options
)
{
    constexpr const char* array_name = "OpenCAX_NodeIds";

    vtkSmartPointer<vtkStringArray> labels =
        make_id_array(
            grid->GetNumberOfPoints(),
            array_name
        );

    grid->GetPointData()->AddArray(labels);

    vtkNew<vtkVertexGlyphFilter> glyph;
    glyph->SetInputData(grid);
    glyph->Update();

    vtkNew<vtkLabeledDataMapper> mapper;
    mapper->SetInputConnection(glyph->GetOutputPort());

    mapper->SetLabelModeToLabelFieldData();
    mapper->SetFieldDataName(array_name);

    mapper->GetLabelTextProperty()->SetFontSize(
        options.node_label_font_size
    );

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
    constexpr const char* array_name = "OpenCAX_CellIds";

    vtkNew<vtkCellCenters> centers;
    centers->SetInputData(grid);
    centers->Update();

    vtkPolyData* center_polydata =
        centers->GetOutput();

    vtkSmartPointer<vtkStringArray> labels =
        make_id_array(
            grid->GetNumberOfCells(),
            array_name
        );

    center_polydata->GetPointData()->AddArray(labels);

    vtkNew<vtkLabeledDataMapper> mapper;
    mapper->SetInputData(center_polydata);

    mapper->SetLabelModeToLabelFieldData();
    mapper->SetFieldDataName(array_name);

    mapper->GetLabelTextProperty()->SetFontSize(
        options.cell_label_font_size
    );

    mapper->GetLabelTextProperty()->SetColor(
        options.cell_label_color.r,
        options.cell_label_color.g,
        options.cell_label_color.b
    );

    vtkNew<vtkActor2D> actor;
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}

static void add_edge_labels(
    vtkRenderer* renderer,
    const Mesh& mesh,
    const MeshTopology& topology,
    const VisualOptions& options
)
{
    constexpr const char* array_name = "OpenCAX_EdgeIds";

    vtkSmartPointer<vtkPolyData> edge_polydata =
        build_edge_polydata_from_topology(
            mesh,
            topology
        );

    vtkNew<vtkCellCenters> centers;
    centers->SetInputData(edge_polydata);
    centers->Update();

    vtkPolyData* center_polydata =
        centers->GetOutput();

    vtkSmartPointer<vtkStringArray> labels =
        make_id_array(
            static_cast<vtkIdType>(topology.num_edges()),
            array_name
        );

    center_polydata->GetPointData()->AddArray(labels);

    vtkNew<vtkLabeledDataMapper> mapper;
    mapper->SetInputData(center_polydata);

    mapper->SetLabelModeToLabelFieldData();
    mapper->SetFieldDataName(array_name);

    mapper->GetLabelTextProperty()->SetFontSize(
        options.edge_label_font_size
    );

    mapper->GetLabelTextProperty()->SetColor(
        options.edge_label_color.r,
        options.edge_label_color.g,
        options.edge_label_color.b
    );

    vtkNew<vtkActor2D> actor;
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}

static void add_axes_widget(
    vtkRenderWindowInteractor* interactor
)
{
    vtkNew<vtkAxesActor> axes;

    vtkNew<vtkOrientationMarkerWidget> widget;
    widget->SetOrientationMarker(axes);
    widget->SetInteractor(interactor);
    widget->SetViewport(
        0.0,
        0.0,
        0.2,
        0.2
    );

    widget->SetEnabled(1);
    widget->InteractiveOff();
}

} // namespace

void VtkViewer::showMesh(
    const Mesh& mesh
)
{
    VisualOptions options;
    showMesh(
        mesh,
        options
    );
}

void VtkViewer::showMeshWithNodeIds(
    const Mesh& mesh
)
{
    VisualOptions options;
    options.show_node_ids = true;
    options.show_cell_ids = false;
    options.show_edge_ids = false;

    showMesh(
        mesh,
        options
    );
}

void VtkViewer::showMeshWithCellIds(
    const Mesh& mesh
)
{
    VisualOptions options;
    options.show_node_ids = false;
    options.show_cell_ids = true;
    options.show_edge_ids = false;

    showMesh(
        mesh,
        options
    );
}

void VtkViewer::showMeshWithEdgeIds(
    const Mesh& mesh
)
{
    VisualOptions options;
    options.show_node_ids = false;
    options.show_cell_ids = false;
    options.show_edge_ids = true;
    options.show_edges = true;

    showMesh(
        mesh,
        options
    );
}

void VtkViewer::showMeshWithIds(
    const Mesh& mesh
)
{
    VisualOptions options;
    options.show_node_ids = true;
    options.show_cell_ids = true;
    options.show_edge_ids = true;
    options.show_edges = true;

    showMesh(
        mesh,
        options
    );
}

void VtkViewer::showMesh(
    const Mesh& mesh,
    const VisualOptions& options
)
{
    vtkSmartPointer<vtkUnstructuredGrid> grid =
        VtkConverter::meshToGrid(mesh);

    MeshTopology topology;

    if (options.show_edges || options.show_edge_ids)
    {
        topology.build(mesh);
    }

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(grid);

    vtkNew<vtkActor> mesh_actor;
    mesh_actor->SetMapper(mapper);

    mesh_actor->GetProperty()->SetColor(
        options.mesh_color.r,
        options.mesh_color.g,
        options.mesh_color.b
    );

    mesh_actor->GetProperty()->SetLineWidth(
        options.line_width
    );

    mesh_actor->GetProperty()->SetPointSize(
        options.point_size
    );

    if (options.wireframe)
    {
        mesh_actor->GetProperty()->SetRepresentationToWireframe();
    }
    else
    {
        mesh_actor->GetProperty()->SetRepresentationToSurface();
    }

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(mesh_actor);

    renderer->SetBackground(
        options.background_color.r,
        options.background_color.g,
        options.background_color.b
    );

    if (options.show_edges)
    {
        add_edge_actor(
            renderer,
            mesh,
            topology,
            options
        );
    }

    if (options.show_nodes)
    {
        add_node_actor(
            renderer,
            grid,
            options
        );
    }

    if (options.show_node_ids)
    {
        add_node_labels(
            renderer,
            grid,
            options
        );
    }

    if (options.show_cell_ids)
    {
        add_cell_labels(
            renderer,
            grid,
            options
        );
    }

    if (options.show_edge_ids)
    {
        add_edge_labels(
            renderer,
            mesh,
            topology,
            options
        );
    }

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);
    window->SetSize(
        options.window_width,
        options.window_height
    );

    window->SetWindowName(
        options.window_title.c_str()
    );

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(window);

    if (options.show_axes)
    {
        add_axes_widget(interactor);
    }

    renderer->ResetCamera();

    window->Render();
    interactor->Start();
}

} // namespace OpenCAX
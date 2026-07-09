#include <OpenCAX/Post/Viewer/VectorFieldViewer.h>

#include <vtkActor.h>
#include <vtkArrowSource.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkGlyph3D.h>
#include <vtkLookupTable.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarActor.h>
#include <vtkTextProperty.h>
#include <vtkTriangle.h>
#include <vtkUnstructuredGrid.h>

#include <cmath>
#include <iostream>

namespace OpenCAX
{

namespace
{

static vtkSmartPointer<vtkUnstructuredGrid> build_triangle_grid(
    const TriangleMesh& mesh
)
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

    return grid;
}

static vtkSmartPointer<vtkPolyData> build_vector_points(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& vx,
    const Eigen::VectorXd& vy
)
{
    vtkNew<vtkPoints> points;

    vtkNew<vtkDoubleArray> vectors;
    vectors->SetName("vectors");
    vectors->SetNumberOfComponents(3);

    vtkNew<vtkDoubleArray> magnitude;
    magnitude->SetName("magnitude");
    magnitude->SetNumberOfComponents(1);

    const auto& nodes =
        mesh.nodes();

    double max_mag = 0.0;

    for (int i = 0; i < vx.size(); ++i)
    {
        const double mag =
            std::sqrt(
                vx(i) * vx(i) +
                vy(i) * vy(i)
            );

        if (mag > max_mag)
        {
            max_mag = mag;
        }
    }

    if (max_mag <= 1.0e-30)
    {
        max_mag = 1.0;
    }

    for (std::size_t i = 0; i < nodes.size(); ++i)
    {
        points->InsertNextPoint(
            nodes[i].x,
            nodes[i].y,
            nodes[i].z + 1.0e-6
        );

        /*
         * 归一化方向，长度由 Glyph scale 控制。
         * 这样箭头方向稳定，不会因为物理量过大导致箭头飞出去。
         */
        const double qx =
            vx(static_cast<int>(i));

        const double qy =
            vy(static_cast<int>(i));

        const double mag =
            std::sqrt(qx * qx + qy * qy);

        if (mag > 1.0e-30)
        {
            vectors->InsertNextTuple3(
                qx / mag,
                qy / mag,
                0.0
            );
        }
        else
        {
            vectors->InsertNextTuple3(
                0.0,
                0.0,
                0.0
            );
        }

        magnitude->InsertNextValue(
            mag
        );
    }

    vtkSmartPointer<vtkPolyData> polydata =
        vtkSmartPointer<vtkPolyData>::New();

    polydata->SetPoints(points);
    polydata->GetPointData()->SetVectors(vectors);
    polydata->GetPointData()->SetScalars(magnitude);

    return polydata;
}

static void add_scalar_bar(
    vtkRenderer* renderer,
    vtkLookupTable* lut,
    const char* title
)
{
    vtkNew<vtkScalarBarActor> scalarBar;

    scalarBar->SetLookupTable(lut);
    scalarBar->SetTitle(title);
    scalarBar->SetNumberOfLabels(6);
    scalarBar->SetLabelFormat("%.3e");

    scalarBar->SetWidth(0.08);
    scalarBar->SetHeight(0.75);
    scalarBar->SetPosition(0.88, 0.12);

    scalarBar->GetTitleTextProperty()->SetColor(
        0.0,
        0.0,
        0.0
    );

    scalarBar->GetTitleTextProperty()->SetFontSize(
        18
    );

    scalarBar->GetLabelTextProperty()->SetColor(
        0.0,
        0.0,
        0.0
    );

    scalarBar->GetLabelTextProperty()->SetFontSize(
        14
    );

    renderer->AddActor2D(
        scalarBar
    );
}

} // namespace

void VectorFieldViewer::showVectorField2D(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& vx,
    const Eigen::VectorXd& vy,
    const char* title,
    double scale_factor
)
{
    showVectorField2DWithMagnitude(
        mesh,
        vx,
        vy,
        title,
        scale_factor
    );
}

void VectorFieldViewer::showVectorField2DWithMagnitude(
    const TriangleMesh& mesh,
    const Eigen::VectorXd& vx,
    const Eigen::VectorXd& vy,
    const char* title,
    double scale_factor
)
{
    if (vx.size() != static_cast<int>(mesh.num_nodes()) ||
        vy.size() != static_cast<int>(mesh.num_nodes()))
    {
        std::cerr
            << "[OpenCAX::VectorFieldViewer] vector size mismatch. "
            << "vx.size() = "
            << vx.size()
            << ", vy.size() = "
            << vy.size()
            << ", mesh.num_nodes() = "
            << mesh.num_nodes()
            << std::endl;

        return;
    }

    vtkSmartPointer<vtkUnstructuredGrid> grid =
        build_triangle_grid(
            mesh
        );

    vtkSmartPointer<vtkPolyData> vector_points =
        build_vector_points(
            mesh,
            vx,
            vy
        );

    /*
     * 背景网格
     */
    vtkNew<vtkDataSetMapper> grid_mapper;
    grid_mapper->SetInputData(grid);
    grid_mapper->ScalarVisibilityOff();

    vtkNew<vtkActor> grid_actor;
    grid_actor->SetMapper(grid_mapper);
    grid_actor->GetProperty()->SetColor(
        0.85,
        0.85,
        0.85
    );

    grid_actor->GetProperty()->SetEdgeVisibility(
        true
    );

    grid_actor->GetProperty()->SetLineWidth(
        1.0
    );

    grid_actor->GetProperty()->SetOpacity(
        0.45
    );

    /*
     * 箭头源
     */
    vtkNew<vtkArrowSource> arrow;
    arrow->SetTipResolution(16);
    arrow->SetShaftResolution(12);
    arrow->SetTipLength(0.30);
    arrow->SetTipRadius(0.10);
    arrow->SetShaftRadius(0.03);

    /*
     * Glyph 箭头
     */
    vtkNew<vtkGlyph3D> glyph;
    glyph->SetSourceConnection(
        arrow->GetOutputPort()
    );

    glyph->SetInputData(
        vector_points
    );

    glyph->SetVectorModeToUseVector();
    glyph->OrientOn();

    glyph->SetScaleModeToScaleByScalar();
    glyph->SetScaleFactor(
        scale_factor
    );

    glyph->Update();

    vtkNew<vtkLookupTable> lut;
    lut->SetNumberOfTableValues(256);
    lut->SetHueRange(0.667, 0.0);
    lut->Build();

    double range[2];
    vector_points->GetPointData()->GetScalars()->GetRange(range);

    vtkNew<vtkPolyDataMapper> arrow_mapper;
    arrow_mapper->SetInputConnection(
        glyph->GetOutputPort()
    );

    arrow_mapper->SetLookupTable(lut);
    arrow_mapper->SetScalarRange(range);
    arrow_mapper->ScalarVisibilityOn();

    vtkNew<vtkActor> arrow_actor;
    arrow_actor->SetMapper(
        arrow_mapper
    );

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(
        grid_actor
    );

    renderer->AddActor(
        arrow_actor
    );

    renderer->SetBackground(
        1.0,
        1.0,
        1.0
    );

    add_scalar_bar(
        renderer,
        lut,
        "|v|"
    );

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(
        renderer
    );

    window->SetWindowName(
        title ? title : "Vector Field"
    );

    window->SetSize(
        1200,
        850
    );

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(
        window
    );

    renderer->ResetCamera();

    window->Render();
    interactor->Start();
}

} // namespace OpenCAX
// #include "MVTKWidget.h"

// MVtkWidget::MVtkWidget(QWidget * parent):
//     QVTKOpenGLNativeWidget(parent)
// {
//     Renderer = vtkSmartPointer<vtkRenderer>::New();
//     Renderer->SetBackground(0.3,0.3,0.3);
//     Renderer->SetBackground2(1.0,1.0,1.0);
//     Renderer->SetGradientBackground(true);
//     renderWindow()->AddRenderer(this->Renderer);

//     vtkNew<MInteractorStyle> interactor;
//     interactor->setHighLightMode(HighLightByBound);
//     interactor->SetDefaultRenderer(this->Renderer);
//     this->interactor()->SetInteractorStyle(interactor);

//     // 初始化角落坐标系
//     vtkNew<vtkAxesActor> axisActor;
//     axisActor->SetShaftType(0);

//     this->AxisWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
//     this->AxisWidget->SetOrientationMarker(axisActor);
//     this->AxisWidget->SetInteractor(renderWindow()->GetInteractor());
//     this->AxisWidget->SetViewport(0.0,0.0,0.1,0.2);
//     this->AxisWidget->SetEnabled(1);
//     this->AxisWidget->InteractiveOff();

//     this->Connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
//     this->WorldPicker = vtkSmartPointer<vtkWorldPointPicker>::New();
//     //Connector->Connect(this->interactor(), vtkCommand::LeftButtonPressEvent, this, SLOT(highLightActor()));

//     ViewAXO();
// }

// MVtkWidget::~MVtkWidget()
// {}

// TopoDS_Shape MVtkWidget::getOCCByActor(vtkActor * actor)
// {
//     IVtkOCC_Shape::Handle iShape = IVtkTools_ShapeObject::GetOccShape(actor);
//     TopoDS_Shape shape = iShape->GetShape();
//     return shape;
// }

// void MVtkWidget::updateRenders()
// {
//     renderWindow()->Render();
// }

// void MVtkWidget::removeAll()
// {
//     vtkActorCollection* actors = Renderer->GetActors();

//     actors->InitTraversal();
//     vtkActor* actor;
//     while((actor = actors->GetNextActor()) != nullptr)
//     {
//         Renderer->RemoveActor(actor);
//     }
//     updateRenders();
// }

// void MVtkWidget::removeActor(vtkActor* actor)
// {
//     Renderer->RemoveActor(actor);
// }

// void MVtkWidget::fitAll()
// {
//     Renderer->ResetCamera();
//     Renderer->ResetCameraClippingRange();
//     updateRenders();
// }

// void MVtkWidget::fitActor(vtkActor * actor)
// {
//     Renderer->ResetCamera(actor->GetBounds());
// }

// void MVtkWidget::disPlayOCC(TopoDS_Shape & aShape)
// {
//     if(aShape.IsNull())
//         qDebug() << "shape err";
//     IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(aShape);
//     vtkNew<IVtkTools_ShapeDataSource> DS;
//     DS->SetShape(aShapeImpl);
//     DS->Update();
//     vtkNew<IVtkTools_DisplayModeFilter> filter;
//     filter->SetInputData(DS->GetOutput());
//     filter->SetDisplayMode(DM_Shading);
//     filter->SetSmoothShading(1);
//     filter->Update();
//     vtkNew<vtkPolyDataMapper> mapper;
//     mapper->SetInputData(filter->GetOutput());

//     vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//     actor->SetMapper(mapper);
//     initStyleActor(actor);
//     Renderer->AddActor(actor);
// }

// void MVtkWidget::disPlayOCCByLowLevel(TopoDS_Shape & aShape)
// {
//     IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(aShape);
//     IVtkVTK_ShapeData::Handle aDataImpl = new IVtkVTK_ShapeData();
//     IVtk_IShapeMesher::Handle aMesher = new IVtkOCC_ShapeMesher();
//     aMesher->Build (aShapeImpl,aDataImpl);
//     vtkPolyData* aPolyData = aDataImpl->getVtkPolyData();
//     vtkNew<vtkPolyDataMapper> mapper;
//     mapper->SetInputData(aPolyData);

//     vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//     actor->SetMapper(mapper);
//     initStyleActor(actor);
//     Renderer->AddActor(actor);
// }

// void MVtkWidget::setEdgeVisibleAll(bool visible)
// {
//     vtkActorCollection* actors = Renderer->GetActors();

//     actors->InitTraversal();
//     vtkActor* actor;
//     while((actor = actors->GetNextActor()) != nullptr)
//     {
//         actor->GetProperty()->SetEdgeVisibility(visible);
//     }
// }

// void MVtkWidget::ViewAXO()
// {
//     Renderer->GetActiveCamera()->SetPosition(0,0,0);
//     Renderer->GetActiveCamera()->SetFocalPoint(-1,-1,-1);
//     Renderer->GetActiveCamera()->SetViewUp(0,0,1);

//     fitAll();
// }

// void MVtkWidget::ViewPositiveX()
// {
//     Renderer->GetActiveCamera()->SetPosition(0,0,0);
//     Renderer->GetActiveCamera()->SetFocalPoint(1,0,0);
//     Renderer->GetActiveCamera()->SetViewUp(0,0,1);

//     fitAll();
// }

// void MVtkWidget::ViewPositiveY()
// {
//     Renderer->GetActiveCamera()->SetPosition(0,0,0);
//     Renderer->GetActiveCamera()->SetFocalPoint(0,1,0);
//     Renderer->GetActiveCamera()->SetViewUp(0,0,1);

//     fitAll();
// }

// void MVtkWidget::ViewPositiveZ()
// {
//     Renderer->GetActiveCamera()->SetPosition(0,0,0);
//     Renderer->GetActiveCamera()->SetFocalPoint(0,0,1);
//     Renderer->GetActiveCamera()->SetViewUp(0,1,0);

//     fitAll();
// }

// void MVtkWidget::ViewNegativeX()
// {
//     Renderer->GetActiveCamera()->SetPosition(0,0,0);
//     Renderer->GetActiveCamera()->SetFocalPoint(-1,0,0);
//     Renderer->GetActiveCamera()->SetViewUp(0,0,1);

//     fitAll();
// }

// void MVtkWidget::ViewNegativeY()
// {
//     Renderer->GetActiveCamera()->SetPosition(0,0,0);
//     Renderer->GetActiveCamera()->SetFocalPoint(0,-1,0);
//     Renderer->GetActiveCamera()->SetViewUp(0,0,1);

//     fitAll();
// }

// void MVtkWidget::ViewNegativeZ()
// {
//     Renderer->GetActiveCamera()->SetPosition(0,0,0);
//     Renderer->GetActiveCamera()->SetFocalPoint(0,0,-1);
//     Renderer->GetActiveCamera()->SetViewUp(0,1,0);

//     fitAll();
// }

// void MVtkWidget::initStyleActor(vtkActor * actor)
// {
//     actor->GetProperty()->SetEdgeVisibility(true);
//     actor->GetProperty()->SetLineWidth(1);
// }

// vtkStandardNewMacro(MInteractorStyle);

// MInteractorStyle::MInteractorStyle():
//     currentHighLightMode(HighLightByColor)
// {
//     picker = vtkSmartPointer<vtkPropPicker>::New();
// }

// MInteractorStyle::~MInteractorStyle()
// {

// }

// void MInteractorStyle::OnLeftButtonDown()
// {
//     highLightBySelect();

//     // Forward events.
//     //vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
// }

// void MInteractorStyle::OnRightButtonDown()
// {
//     if(this->Interactor->GetShiftKey())
//     {
//         this->StartRotate();
//     } else
//     {
//         vtkInteractorStyleTrackballCamera::OnRightButtonDown();
//     }
// }

// void MInteractorStyle::OnRightButtonUp()
// {
//     if(this->State == VTKIS_ROTATE)
//     {
//         this->EndRotate();
//     } else
//     {
//         vtkInteractorStyleTrackballCamera::OnRightButtonUp();
//     }
// }

// void MInteractorStyle::OnMouseMove()
// {
//     if (State != VTKIS_ROTATE && State != VTKIS_PAN)
//         highLightByDetect();
//     vtkInteractorStyleTrackballCamera::OnMouseMove();
// }

// void MInteractorStyle::setHighLightMode(HighLightMode mode)
// {
//     currentHighLightMode = mode;
// }

// void MInteractorStyle::highLightBySelect()
// {
//     int* clickPos = this->GetInteractor()->GetEventPosition();
//     picker->Pick(clickPos[0],clickPos[1],0,this->GetDefaultRenderer());
//     vtkSmartPointer<vtkActor> pickedActor = vtkSmartPointer<vtkActor>::New();
//     pickedActor = picker->GetActor();
//     if(pickedActor)
//     {
//         auto it = std::find(pickedActors.begin(),pickedActors.end(),pickedActor);
//         if(it != pickedActors.end() && this->Interactor->GetControlKey())
//         {
//             int i = it- pickedActors.begin();
//             pickedActor->GetProperty()->DeepCopy(pickedPropertys[i]);
//             pickedActors.erase(it);
//             pickedPropertys.erase(pickedPropertys.begin() + i);
//         }
//         else if (it == pickedActors.end())
//         {
//             pickedActors.push_back(pickedActor);
//             vtkSmartPointer<vtkProperty> pickedProperty = vtkSmartPointer<vtkProperty>::New();
//             pickedProperty->DeepCopy(pickedActor->GetProperty());
//             pickedPropertys.push_back(pickedProperty);

//             vtkNew<vtkNamedColors> colors;
//             pickedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
//             pickedActor->GetProperty()->SetDiffuse(1.0);
//             pickedActor->GetProperty()->SetSpecular(0.0);
//             pickedActor->GetProperty()->EdgeVisibilityOn();
//         }
//     }
//     else
//     {
//         int i = 0;
//         for(auto pickedActor: pickedActors)
//         {
//             pickedActor->GetProperty()->DeepCopy(pickedPropertys[i]);
//             ++i;
//         }
//         pickedActors.clear();
//         pickedPropertys.clear();
//     }

//     this->GetDefaultRenderer()->GetRenderWindow()->Render();
// }

// void MInteractorStyle::highLightByDetect()
// {
//     int* clickPos = this->GetInteractor()->GetEventPosition();
//     picker->Pick(clickPos[0],clickPos[1],0,this->GetDefaultRenderer());
//     vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();

//     // 如果检测到的 actor 没有变化，则不进行任何操作
//     if(pickedActor == detectActor)
//     {
//         return;
//     }

//     // 移除之前的高亮 actor
//     if(detectActor)
//     {
//         this->GetDefaultRenderer()->RemoveActor(detectActor);
//         detectActor = nullptr;
//     }

//     if(pickedActor)
//     {
//         vtkNew<vtkOutlineSource> outlineSource;
//         outlineSource->SetBounds(pickedActor->GetBounds());
//         vtkNew<vtkPolyDataMapper> outlineMapper;
//         outlineMapper->SetInputConnection(outlineSource->GetOutputPort());
//         detectActor = vtkSmartPointer<vtkActor>::New();
//         detectActor->SetMapper(outlineMapper);
//         detectActor->GetProperty()->SetColor(1.0,0.0,0.0);
//         detectActor->GetProperty()->SetLineWidth(2);
//         this->GetDefaultRenderer()->AddActor(detectActor);
//     }

//     this->GetDefaultRenderer()->GetRenderWindow()->Render();
// }

#include "MVkWidget.h"

#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkOutlineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>

vtkStandardNewMacro(MInteractorStyle);

// ================= MInteractorStyle =================

MInteractorStyle::MInteractorStyle()
{
    Picker = vtkSmartPointer<vtkPropPicker>::New();
}

void MInteractorStyle::SetDefaultRenderer(vtkRenderer* ren)
{
    this->Superclass::SetDefaultRenderer(ren);
}

void MInteractorStyle::OnLeftButtonDown()
{
    highlightSelect();
}

void MInteractorStyle::OnRightButtonDown()
{
    this->StartRotate();
}

void MInteractorStyle::OnMouseMove()
{
    highlightHover();
    this->Superclass::OnMouseMove();
}

void MInteractorStyle::highlightSelect()
{
    int* pos = this->GetInteractor()->GetEventPosition();
    Picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());
    vtkActor* actor = Picker->GetActor();
    if (!actor) return;

    vtkNew<vtkProperty> backup;
    backup->DeepCopy(actor->GetProperty());

    actor->GetProperty()->SetColor(1, 0, 0);
    actor->GetProperty()->EdgeVisibilityOn();

    SelectedActors.push_back(actor);
    BackupProperty.push_back(backup);

    this->GetDefaultRenderer()->GetRenderWindow()->Render();
}

void MInteractorStyle::highlightHover()
{
    int* pos = this->GetInteractor()->GetEventPosition();
    Picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());
    vtkActor* actor = Picker->GetActor();

    if (!actor) return;

    vtkNew<vtkOutlineSource> outline;
    outline->SetBounds(actor->GetBounds());

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(outline->GetOutputPort());

    if (!HoverOutline)
        HoverOutline = vtkSmartPointer<vtkActor>::New();

    HoverOutline->SetMapper(mapper);
    HoverOutline->GetProperty()->SetColor(1, 1, 0);
    HoverOutline->GetProperty()->SetLineWidth(2);

    this->GetDefaultRenderer()->AddActor(HoverOutline);
    this->GetDefaultRenderer()->GetRenderWindow()->Render();
}

// ================= MVtkWidget =================

MVtkWidget::MVtkWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
{
    initRenderer();
    initInteractor();
    initViewCube();
    viewAXO();
}

MVtkWidget::~MVtkWidget() {}

void MVtkWidget::initRenderer()
{
    Renderer = vtkSmartPointer<vtkRenderer>::New();
    Renderer->SetBackground(0.25, 0.25, 0.25);
    this->renderWindow()->AddRenderer(Renderer);
}

void MVtkWidget::initInteractor()
{
    InteractorStyle = vtkSmartPointer<MInteractorStyle>::New();
    InteractorStyle->SetDefaultRenderer(Renderer);
    this->interactor()->SetInteractorStyle(InteractorStyle);
}

void MVtkWidget::initViewCube()
{
    ViewCube = vtkSmartPointer<vtkAnnotatedCubeActor>::New();
    ViewCube->SetXPlusFaceText("X+");
    ViewCube->SetXMinusFaceText("X-");
    ViewCube->SetYPlusFaceText("Y+");
    ViewCube->SetYMinusFaceText("Y-");
    ViewCube->SetZPlusFaceText("Z+");
    ViewCube->SetZMinusFaceText("Z-");

    CubeWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    CubeWidget->SetOrientationMarker(ViewCube);
    CubeWidget->SetInteractor(this->interactor());
    CubeWidget->SetViewport(0.8, 0.0, 1.0, 0.2);
    CubeWidget->SetEnabled(1);
    CubeWidget->InteractiveOn();
}

void MVtkWidget::displayOCC(const TopoDS_Shape& shape)
{
    IVtkOCC_Shape::Handle impl = new IVtkOCC_Shape(shape);

    vtkNew<IVtkTools_ShapeDataSource> ds;
    ds->SetShape(impl);
    ds->Update();

    vtkNew<IVtkTools_DisplayModeFilter> filter;
    filter->SetInputData(ds->GetOutput());
    filter->SetDisplayMode(DM_Shading);
    filter->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(filter->GetOutput());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->EdgeVisibilityOn();

    Renderer->AddActor(actor);
    resetCamera();
}

void MVtkWidget::clear()
{
    Renderer->RemoveAllViewProps();
    resetCamera();
}

void MVtkWidget::resetCamera()
{
    Renderer->ResetCamera();
    renderWindow()->Render();
}

// ================= 视角 =================

void MVtkWidget::viewAXO()
{
    auto cam = Renderer->GetActiveCamera();
    cam->SetPosition(1, 1, 1);
    cam->SetFocalPoint(0, 0, 0);
    cam->SetViewUp(0, 0, 1);
    resetCamera();
}

void MVtkWidget::viewXPos() { Renderer->GetActiveCamera()->SetPosition(1,0,0); resetCamera(); }
void MVtkWidget::viewXNeg() { Renderer->GetActiveCamera()->SetPosition(-1,0,0); resetCamera(); }
void MVtkWidget::viewYPos() { Renderer->GetActiveCamera()->SetPosition(0,1,0); resetCamera(); }
void MVtkWidget::viewYNeg() { Renderer->GetActiveCamera()->SetPosition(0,-1,0); resetCamera(); }
void MVtkWidget::viewZPos() { Renderer->GetActiveCamera()->SetPosition(0,0,1); resetCamera(); }
void MVtkWidget::viewZNeg() { Renderer->GetActiveCamera()->SetPosition(0,0,-1); resetCamera(); }


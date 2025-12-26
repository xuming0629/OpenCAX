#include "MVtkWidget.h"
#include <QDebug>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>
#include <vtkCamera.h>
#include <algorithm>

vtkStandardNewMacro(MInteractorStyle);

// ==================== MInteractorStyle ====================
MInteractorStyle::MInteractorStyle()
    : currentHighLightMode(HighLightByColor)
{
    picker = vtkSmartPointer<vtkPropPicker>::New();
}

MInteractorStyle::~MInteractorStyle() {}

void MInteractorStyle::setHighLightMode(HighLightMode mode) {
    currentHighLightMode = mode;
}

void MInteractorStyle::OnLeftButtonDown() { highLightBySelect(); }

void MInteractorStyle::OnRightButtonDown() {
    if (this->Interactor->GetShiftKey())
        this->StartRotate();
    else
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}

void MInteractorStyle::OnRightButtonUp() {
    if (this->State == VTKIS_ROTATE)
        this->EndRotate();
    else
        vtkInteractorStyleTrackballCamera::OnRightButtonUp();
}

void MInteractorStyle::OnMouseMove() {
    if (State != VTKIS_ROTATE && State != VTKIS_PAN)
        highLightByDetect();
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void MInteractorStyle::highLightBySelect() {
    int* clickPos = this->GetInteractor()->GetEventPosition();
    picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
    vtkActor* pickedActor = picker->GetActor();
    bool ctrlPressed = this->Interactor->GetControlKey();

    if (pickedActor) {
        auto it = std::find(pickedActors.begin(), pickedActors.end(), pickedActor);
        if (it != pickedActors.end() && ctrlPressed) {
            int index = static_cast<int>(it - pickedActors.begin());
            pickedActor->GetProperty()->DeepCopy(pickedPropertys[index]);
            pickedActors.erase(it);
            pickedPropertys.erase(pickedPropertys.begin() + index);
        } else if (it == pickedActors.end()) {
            if (!ctrlPressed) {
                for (size_t i = 0; i < pickedActors.size(); ++i)
                    pickedActors[i]->GetProperty()->DeepCopy(pickedPropertys[i]);
                pickedActors.clear();
                pickedPropertys.clear();
            }
            vtkSmartPointer<vtkProperty> originProp = vtkSmartPointer<vtkProperty>::New();
            originProp->DeepCopy(pickedActor->GetProperty());
            pickedActors.push_back(pickedActor);
            pickedPropertys.push_back(originProp);

            vtkNew<vtkNamedColors> colors;
            pickedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
            pickedActor->GetProperty()->EdgeVisibilityOn();
            pickedActor->GetProperty()->SetLineWidth(2.0);
        }
    } else {
        for (size_t i = 0; i < pickedActors.size(); ++i)
            pickedActors[i]->GetProperty()->DeepCopy(pickedPropertys[i]);
        pickedActors.clear();
        pickedPropertys.clear();
    }

    this->GetDefaultRenderer()->GetRenderWindow()->Render();
}

void MInteractorStyle::highLightByDetect() {
    int* clickPos = this->GetInteractor()->GetEventPosition();
    picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
    vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();

    if (pickedActor == detectActor) return;
    if (detectActor) this->GetDefaultRenderer()->RemoveActor(detectActor);

    if (pickedActor) {
        vtkNew<vtkOutlineSource> outlineSource;
        outlineSource->SetBounds(pickedActor->GetBounds());
        vtkNew<vtkPolyDataMapper> outlineMapper;
        outlineMapper->SetInputConnection(outlineSource->GetOutputPort());
        detectActor = vtkSmartPointer<vtkActor>::New();
        detectActor->SetMapper(outlineMapper);
        detectActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        detectActor->GetProperty()->SetLineWidth(2);
        this->GetDefaultRenderer()->AddActor(detectActor);
    }

    this->GetDefaultRenderer()->GetRenderWindow()->Render();
}

// ==================== MVtkWidget ====================
MVtkWidget::MVtkWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
{
    Renderer = vtkSmartPointer<vtkRenderer>::New();
    Renderer->SetBackground(0.3,0.3,0.3);
    Renderer->SetBackground2(1.0,1.0,1.0);
    Renderer->SetGradientBackground(true);
    renderWindow()->AddRenderer(Renderer);

    vtkNew<MInteractorStyle> interactor;
    interactor->setHighLightMode(HighLightByBound);
    interactor->SetDefaultRenderer(Renderer);
    this->interactor()->SetInteractorStyle(interactor);

    initOrientationMarker();

    Connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    WorldPicker = vtkSmartPointer<vtkWorldPointPicker>::New();



    ViewAXO();
}

MVtkWidget::~MVtkWidget() {}

void MVtkWidget::initOrientationMarker() {
    vtkNew<vtkAxesActor> axisActor;
    axisActor->SetShaftType(0);

    AxisWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    AxisWidget->SetOrientationMarker(axisActor);
    AxisWidget->SetInteractor(renderWindow()->GetInteractor());
    AxisWidget->SetViewport(0.0,0.0,0.12,0.2);
    AxisWidget->SetEnabled(1);
    AxisWidget->InteractiveOff();
}

// 下面函数和你原来的完全一致
TopoDS_Shape MVtkWidget::getOCCByActor(vtkActor* actor) {
    if(!actor) return TopoDS_Shape();
    IVtkOCC_Shape::Handle iShape = IVtkTools_ShapeObject::GetOccShape(actor);
    if(iShape.IsNull()) return TopoDS_Shape();
    return iShape->GetShape();
}

void MVtkWidget::updateRenders() { renderWindow()->Render(); }
void MVtkWidget::removeAll() {
    vtkActorCollection* actors = Renderer->GetActors();
    actors->InitTraversal();
    vtkActor* actor;
    while ((actor = actors->GetNextActor()) != nullptr) Renderer->RemoveActor(actor);
    updateRenders();
}
void MVtkWidget::removeActor(vtkActor* actor) { Renderer->RemoveActor(actor); }
void MVtkWidget::fitAll() { Renderer->ResetCamera(); Renderer->ResetCameraClippingRange(); updateRenders(); }
void MVtkWidget::fitActor(vtkActor* actor) { Renderer->ResetCamera(actor->GetBounds()); }

void MVtkWidget::disPlayOCC(TopoDS_Shape& aShape) {
    currentShape = aShape;
    if(aShape.IsNull()) { qDebug()<<"shape err"; return; }
    IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(aShape);
    vtkNew<IVtkTools_ShapeDataSource> DS;
    DS->SetShape(aShapeImpl);
    DS->Update();
    vtkNew<IVtkTools_DisplayModeFilter> filter;
    filter->SetInputData(DS->GetOutput());
    filter->SetDisplayMode(DM_Shading);
    filter->SetSmoothShading(1);
    filter->Update();
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(filter->GetOutput());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    initStyleActor(actor);
    Renderer->AddActor(actor);
}

void MVtkWidget::disPlayOCCByLowLevel(TopoDS_Shape& aShape) {
    IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(aShape);
    IVtkVTK_ShapeData::Handle aDataImpl = new IVtkVTK_ShapeData();
    IVtk_IShapeMesher::Handle aMesher = new IVtkOCC_ShapeMesher();
    aMesher->Build(aShapeImpl,aDataImpl);
    vtkPolyData* aPolyData = aDataImpl->getVtkPolyData();
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(aPolyData);
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    initStyleActor(actor);
    Renderer->AddActor(actor);
}

void MVtkWidget::setEdgeVisibleAll(bool visible) {
    vtkActorCollection* actors = Renderer->GetActors();
    actors->InitTraversal();
    vtkActor* actor;
    while ((actor=actors->GetNextActor())!=nullptr)
        actor->GetProperty()->SetEdgeVisibility(visible);
}

void MVtkWidget::initStyleActor(vtkActor* actor) {
    actor->GetProperty()->SetEdgeVisibility(true);
    actor->GetProperty()->SetLineWidth(1);
}

#define VIEW_CAMERA(posX,posY,posZ,focusX,focusY,focusZ,upX,upY,upZ) \
{ \
        Renderer->GetActiveCamera()->SetPosition(posX,posY,posZ); \
        Renderer->GetActiveCamera()->SetFocalPoint(focusX,focusY,focusZ); \
        Renderer->GetActiveCamera()->SetViewUp(upX,upY,upZ); \
        fitAll(); \
}

void MVtkWidget::ViewAXO()      { VIEW_CAMERA(0,0,0,-1,-1,-1,0,0,1) }
void MVtkWidget::ViewPositiveX(){ VIEW_CAMERA(0,0,0,1,0,0,0,0,1) }
void MVtkWidget::ViewPositiveY(){ VIEW_CAMERA(0,0,0,0,1,0,0,0,1) }
void MVtkWidget::ViewPositiveZ(){ VIEW_CAMERA(0,0,0,0,0,1,0,1,0) }
void MVtkWidget::ViewNegativeX(){ VIEW_CAMERA(0,0,0,-1,0,0,0,0,1) }
void MVtkWidget::ViewNegativeY(){ VIEW_CAMERA(0,0,0,0,-1,0,0,0,1) }
void MVtkWidget::ViewNegativeZ(){ VIEW_CAMERA(0,0,0,0,0,-1,0,1,0) }



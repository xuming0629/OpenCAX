#pragma once

#include <QWidget>
#include "QVTKOpenGLNativeWidget.h"

#include <vtkAnnotatedCubeActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkTextProperty.h>
#include <vtkCallbackCommand.h>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkWorldPointPicker.h>
#include <vtkPropPicker.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkOutlineSource.h>
#include <vtkNamedColors.h>
#include <vtkCamera.h>


#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <IVtk_IShapeMesher.hxx>
#include <IVtkOCC_ShapeMesher.hxx>
#include <IVtkTools_ShapeObject.hxx>

enum HighLightMode {
    HighLightByColor,
    HighLightByBound
};

class MInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static MInteractorStyle* New();
    vtkTypeMacro(MInteractorStyle, vtkInteractorStyleTrackballCamera);

    MInteractorStyle();
    ~MInteractorStyle() override;

    void setHighLightMode(HighLightMode mode);

    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override {}
    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    void OnMouseMove() override;

private:
    void highLightBySelect();
    void highLightByDetect();

    vtkSmartPointer<vtkPropPicker> picker;
    HighLightMode currentHighLightMode;
    std::vector<vtkSmartPointer<vtkActor>> pickedActors;
    std::vector<vtkSmartPointer<vtkProperty>> pickedPropertys;
    vtkSmartPointer<vtkActor> detectActor;
};

class MVtkWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit MVtkWidget(QWidget* parent = nullptr);
    ~MVtkWidget() override;

    // Actor 操作
    TopoDS_Shape getOCCByActor(vtkActor* actor);
    void updateRenders();
    void removeAll();
    void removeActor(vtkActor* actor);
    void fitAll();
    void fitActor(vtkActor* actor);
    void disPlayOCC(TopoDS_Shape& aShape);
    void disPlayOCCByLowLevel(TopoDS_Shape& aShape);
    void setEdgeVisibleAll(bool visible);

    // 视角切换
    void ViewAXO();
    void ViewPositiveX();
    void ViewPositiveY();
    void ViewPositiveZ();
    void ViewNegativeX();
    void ViewNegativeY();
    void ViewNegativeZ();

    void ViewTop();
    void ViewBottom();
    void ViewLeft();
    void ViewRight();
    void ViewFront();
    void ViewBack();
    TopoDS_Shape currentShape;

private:
    void initStyleActor(vtkActor* actor);
    void initOrientationMarker(); // 初始化 ViewCube / 坐标轴

private:
    vtkSmartPointer<vtkRenderer> Renderer;
    vtkSmartPointer<vtkOrientationMarkerWidget> AxisWidget;
    vtkSmartPointer<vtkEventQtSlotConnect> Connector;
    vtkSmartPointer<vtkWorldPointPicker> WorldPicker;
    vtkSmartPointer<vtkActor> HighLightedActor;

};

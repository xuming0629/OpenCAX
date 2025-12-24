#pragma once

#include "QVTKOpenGLNativeWidget.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkAxesActor.h"
#include "vtkCaptionActor2D.h"
#include "vtkTextProperty.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkWorldPointPicker.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkOutlineSource.h"
#include "vtkNamedColors.h"
#include "vtkPropPicker.h"
#include "vtkCamera.h"

#include "vtkAnnotatedCubeActor.h"

#include "TopoDS_Shape.hxx"
#include "IVtkOCC_Shape.hxx"
#include "IVtkTools_ShapeDataSource.hxx"
#include "IVtkTools_DisplayModeFilter.hxx"
#include "IVtk_IShapeMesher.hxx"
#include "IVtkOCC_ShapeMesher.hxx"
#include "IVtkTools_ShapeObject.hxx"

class MVtkWidget:public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    MVtkWidget(QWidget* parent = nullptr);
    ~MVtkWidget();

    TopoDS_Shape getOCCByActor(vtkActor* actor);

    void updateRenders();
    void removeAll();
    void removeActor(vtkActor* actor);

    void fitAll();
    void fitActor(vtkActor* actor);

    void disPlayOCC(TopoDS_Shape& aShape);
    void disPlayOCCByLowLevel(TopoDS_Shape& aShape);

    void setEdgeVisibleAll(bool visible);

    void ViewAXO();
    void ViewPositiveX();
    void ViewPositiveY();
    void ViewPositiveZ();
    void ViewNegativeX();
    void ViewNegativeY();
    void ViewNegativeZ();

private:
    void initStyleActor(vtkActor* actor);

    vtkSmartPointer<vtkRenderer> Renderer;
    vtkSmartPointer<vtkOrientationMarkerWidget> AxisWidget;
    vtkSmartPointer<vtkEventQtSlotConnect> Connector;
    vtkSmartPointer<vtkWorldPointPicker> WorldPicker;
    vtkSmartPointer<vtkActor> HighLightedActor;
};

enum HighLightMode{
    HighLightByColor,
    HighLightByBound
};

class MInteractorStyle: public vtkInteractorStyleTrackballCamera
{
public:
    static MInteractorStyle* New();

    vtkTypeMacro(MInteractorStyle, vtkInteractorStyleTrackballCamera);

    MInteractorStyle();

    virtual ~MInteractorStyle();

    virtual void OnLeftButtonDown() override;
    virtual void OnLeftButtonUp() override{}
    virtual void OnRightButtonDown() override;
    virtual void OnRightButtonUp() override;
    virtual void OnMouseMove() override;

    void setHighLightMode(HighLightMode mode);
    void highLightBySelect();
    void highLightByDetect();

private:
    vtkSmartPointer<vtkPropPicker> picker;
    HighLightMode currentHighLightMode;
    std::vector<vtkSmartPointer<vtkActor>> pickedActors;
    std::vector<vtkSmartPointer<vtkProperty>> pickedPropertys;
    vtkSmartPointer<vtkActor> detectActor;
};

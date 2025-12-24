#include "MVTKWidget.h"

MVtkWidget::MVtkWidget(QWidget * parent):
    QVTKOpenGLNativeWidget(parent)
{
    /**********************************************************************
     * 1. Renderer 初始化（三维场景的“世界容器”）
     *
     * vtkRenderer 是 VTK 场景管理的核心对象：
     *  - 管理所有 vtkActor（几何体）
     *  - 管理 vtkCamera（相机）
     *  - 管理光照（Light）
     *
     * 一个 RenderWindow 可以有多个 Renderer（分屏 / 视口）
     **********************************************************************/
    // 创建渲染器实例（使用智能指针，VTK 内存自动管理）
    Renderer = vtkSmartPointer<vtkRenderer>::New();
    Renderer->SetBackground(0.3,0.3,0.3);       // 设置背景颜色（深灰色，工业软件常用）
    Renderer->SetBackground2(1.0,1.0,1.0);      // 设置渐变背景的第二种颜色（浅色）

    // 开启渐变背景模式
    // 视觉效果更柔和，减少长时间观察的视觉疲劳
    Renderer->SetGradientBackground(true);

    // 将 Renderer 添加到当前 Qt-VTK 渲染窗口
    // QVTKOpenGLNativeWidget 内部已经创建了 vtkRenderWindow
    renderWindow()->AddRenderer(this->Renderer);

    // 至此：
    // RenderWindow
    //    └── Renderer
    //          ├── Camera
    //          └── Actors

    /**********************************************************************
     * 2. 自定义交互器（InteractorStyle）
     *
     * 作用：
     *  - 接管鼠标/键盘事件
     *  - 实现拾取、高亮、旋转、平移等交互逻辑
     *
     * 将交互逻辑与视图（MVtkWidget）解耦，
     * 是 CAD / 三维软件的标准设计方式
     **********************************************************************/

    // 创建自定义交互器（继承自 vtkInteractorStyleTrackballCamera）
    vtkNew<MInteractorStyle> interactor;
    // 设置高亮模式（例如：颜色高亮 / 包围盒高亮）
    interactor->setHighLightMode(HighLightByBound);
    // 设置默认渲染器
    // 非常重要：Pick / Render 都依赖 DefaultRenderer
    interactor->SetDefaultRenderer(this->Renderer);
    // 将自定义交互器安装到 Qt-VTK 交互系统中
    this->interactor()->SetInteractorStyle(interactor);

    /**********************************************************************
     * 3. 右下角坐标轴（Orientation Marker）
     *
     * 工业三维软件标配功能：
     *  - 显示世界坐标方向
     *  - 辅助用户空间感知
     *
     * 常见于：SolidWorks / CATIA / NX / Creo
     **********************************************************************/
    // 创建坐标轴 Actor（X/Y/Z）
    vtkNew<vtkAxesActor> axisActor;
    // 设置坐标轴样式
    // ShaftType = 0 通常为线段样式（简洁）
    axisActor->SetShaftType(0);

    // 创建方向标记 Widget
    this->AxisWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    // 将坐标轴 Actor 设置为方向标记内容
    this->AxisWidget->SetOrientationMarker(axisActor);
    // 设置交互器（必须与主 RenderWindow 的 Interactor 绑定）
    this->AxisWidget->SetInteractor(renderWindow()->GetInteractor());
    // 设置坐标轴在窗口中的显示区域（归一化坐标）
    // (xmin, ymin, xmax, ymax)
    // 这里放在左下角
    this->AxisWidget->SetViewport(0.0,0.0,0.1,0.2);
    this->AxisWidget->SetEnabled(1);        // 启用坐标轴 Widget

    // 禁止用户直接拖拽坐标轴
    // 避免误操作，保持固定位置
    this->AxisWidget->InteractiveOff();

    /**********************************************************************
     * 4. Qt - VTK 事件桥接 & 拾取器初始化
     *
     * 这些对象为后续功能预留：
     *  - Qt 信号槽 ↔ VTK 事件
     *  - 世界坐标拾取（点、线、面）
     **********************************************************************/
    // Qt 与 VTK 事件连接器（可将 VTK 事件转为 Qt Slot）
    this->Connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
    // 世界坐标拾取器
    // 常用于获取鼠标点击位置对应的三维坐标
    this->WorldPicker = vtkSmartPointer<vtkWorldPointPicker>::New();
    // 示例（当前未启用）：
    // Connector->Connect(
    //     this->interactor(),
    //     vtkCommand::LeftButtonPressEvent,
    //     this,
    //     SLOT(highLightActor())
    // );


    /**********************************************************************
     * 5. 初始化默认视角（轴测视图）
     *
     * AXO（Axonometric View）：
     *  - 工业建模最常用的初始视角
     *  - 同时看到 X/Y/Z 三个方向
     **********************************************************************/

    ViewAXO();
}

MVtkWidget::~MVtkWidget()
{}

TopoDS_Shape MVtkWidget::getOCCByActor(vtkActor * actor)
{
    IVtkOCC_Shape::Handle iShape = IVtkTools_ShapeObject::GetOccShape(actor);
    TopoDS_Shape shape = iShape->GetShape();
    return shape;
}

void MVtkWidget::updateRenders()
{
    renderWindow()->Render();
}

void MVtkWidget::removeAll()
{
    vtkActorCollection* actors = Renderer->GetActors();

    actors->InitTraversal();
    vtkActor* actor;
    while((actor = actors->GetNextActor()) != nullptr)
    {
        Renderer->RemoveActor(actor);
    }
    updateRenders();
}

void MVtkWidget::removeActor(vtkActor* actor)
{
    Renderer->RemoveActor(actor);
}

void MVtkWidget::fitAll()
{
    Renderer->ResetCamera();
    Renderer->ResetCameraClippingRange();
    updateRenders();
}

void MVtkWidget::fitActor(vtkActor * actor)
{
    Renderer->ResetCamera(actor->GetBounds());
}

void MVtkWidget::disPlayOCC(TopoDS_Shape & aShape)
{
    if(aShape.IsNull())
        qDebug() << "shape err";
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

void MVtkWidget::disPlayOCCByLowLevel(TopoDS_Shape & aShape)
{
    IVtkOCC_Shape::Handle aShapeImpl = new IVtkOCC_Shape(aShape);
    IVtkVTK_ShapeData::Handle aDataImpl = new IVtkVTK_ShapeData();
    IVtk_IShapeMesher::Handle aMesher = new IVtkOCC_ShapeMesher();
    aMesher->Build (aShapeImpl,aDataImpl);
    vtkPolyData* aPolyData = aDataImpl->getVtkPolyData();
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(aPolyData);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    initStyleActor(actor);
    Renderer->AddActor(actor);
}

void MVtkWidget::setEdgeVisibleAll(bool visible)
{
    vtkActorCollection* actors = Renderer->GetActors();

    actors->InitTraversal();
    vtkActor* actor;
    while((actor = actors->GetNextActor()) != nullptr)
    {
        actor->GetProperty()->SetEdgeVisibility(visible);
    }
}

void MVtkWidget::ViewAXO()
{
    Renderer->GetActiveCamera()->SetPosition(0,0,0);
    Renderer->GetActiveCamera()->SetFocalPoint(-1,-1,-1);
    Renderer->GetActiveCamera()->SetViewUp(0,0,1);

    fitAll();
}

void MVtkWidget::ViewPositiveX()
{
    Renderer->GetActiveCamera()->SetPosition(0,0,0);
    Renderer->GetActiveCamera()->SetFocalPoint(1,0,0);
    Renderer->GetActiveCamera()->SetViewUp(0,0,1);

    fitAll();
}

void MVtkWidget::ViewPositiveY()
{
    Renderer->GetActiveCamera()->SetPosition(0,0,0);
    Renderer->GetActiveCamera()->SetFocalPoint(0,1,0);
    Renderer->GetActiveCamera()->SetViewUp(0,0,1);

    fitAll();
}

void MVtkWidget::ViewPositiveZ()
{
    Renderer->GetActiveCamera()->SetPosition(0,0,0);
    Renderer->GetActiveCamera()->SetFocalPoint(0,0,1);
    Renderer->GetActiveCamera()->SetViewUp(0,1,0);

    fitAll();
}

void MVtkWidget::ViewNegativeX()
{
    Renderer->GetActiveCamera()->SetPosition(0,0,0);
    Renderer->GetActiveCamera()->SetFocalPoint(-1,0,0);
    Renderer->GetActiveCamera()->SetViewUp(0,0,1);

    fitAll();
}

void MVtkWidget::ViewNegativeY()
{
    Renderer->GetActiveCamera()->SetPosition(0,0,0);
    Renderer->GetActiveCamera()->SetFocalPoint(0,-1,0);
    Renderer->GetActiveCamera()->SetViewUp(0,0,1);

    fitAll();
}

void MVtkWidget::ViewNegativeZ()
{
    Renderer->GetActiveCamera()->SetPosition(0,0,0);
    Renderer->GetActiveCamera()->SetFocalPoint(0,0,-1);
    Renderer->GetActiveCamera()->SetViewUp(0,1,0);

    fitAll();
}

void MVtkWidget::initStyleActor(vtkActor * actor)
{
    actor->GetProperty()->SetEdgeVisibility(true);
    actor->GetProperty()->SetLineWidth(1);
}

vtkStandardNewMacro(MInteractorStyle);

MInteractorStyle::MInteractorStyle():
    currentHighLightMode(HighLightByColor)
{
    picker = vtkSmartPointer<vtkPropPicker>::New();
}

MInteractorStyle::~MInteractorStyle()
{

}

void MInteractorStyle::OnLeftButtonDown()
{
    highLightBySelect();

    // Forward events.
    //vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void MInteractorStyle::OnRightButtonDown()
{
    if(this->Interactor->GetShiftKey())
    {
        this->StartRotate();
    } else
    {
        vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }
}

void MInteractorStyle::OnRightButtonUp()
{
    if(this->State == VTKIS_ROTATE)
    {
        this->EndRotate();
    } else
    {
        vtkInteractorStyleTrackballCamera::OnRightButtonUp();
    }
}

void MInteractorStyle::OnMouseMove()
{
    if (State != VTKIS_ROTATE && State != VTKIS_PAN)
        highLightByDetect();
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void MInteractorStyle::setHighLightMode(HighLightMode mode)
{
    currentHighLightMode = mode;
}

void MInteractorStyle::highLightBySelect()
{
    int* clickPos = this->GetInteractor()->GetEventPosition();
    picker->Pick(clickPos[0],clickPos[1],0,this->GetDefaultRenderer());
    vtkSmartPointer<vtkActor> pickedActor = vtkSmartPointer<vtkActor>::New();
    pickedActor = picker->GetActor();
    if(pickedActor)
    {
        auto it = std::find(pickedActors.begin(),pickedActors.end(),pickedActor);
        if(it != pickedActors.end() && this->Interactor->GetControlKey())
        {
            int i = it- pickedActors.begin();
            pickedActor->GetProperty()->DeepCopy(pickedPropertys[i]);
            pickedActors.erase(it);
            pickedPropertys.erase(pickedPropertys.begin() + i);
        }
        else if (it == pickedActors.end())
        {
            pickedActors.push_back(pickedActor);
            vtkSmartPointer<vtkProperty> pickedProperty = vtkSmartPointer<vtkProperty>::New();
            pickedProperty->DeepCopy(pickedActor->GetProperty());
            pickedPropertys.push_back(pickedProperty);

            vtkNew<vtkNamedColors> colors;
            pickedActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
            pickedActor->GetProperty()->SetDiffuse(1.0);
            pickedActor->GetProperty()->SetSpecular(0.0);
            pickedActor->GetProperty()->EdgeVisibilityOn();
        }
    }
    else
    {
        int i = 0;
        for(auto pickedActor: pickedActors)
        {
            pickedActor->GetProperty()->DeepCopy(pickedPropertys[i]);
            ++i;
        }
        pickedActors.clear();
        pickedPropertys.clear();
    }

    this->GetDefaultRenderer()->GetRenderWindow()->Render();
}

void MInteractorStyle::highLightByDetect()
{
    int* clickPos = this->GetInteractor()->GetEventPosition();
    picker->Pick(clickPos[0],clickPos[1],0,this->GetDefaultRenderer());
    vtkSmartPointer<vtkActor> pickedActor = picker->GetActor();

    // 如果检测到的 actor 没有变化，则不进行任何操作
    if(pickedActor == detectActor)
    {
        return;
    }

    // 移除之前的高亮 actor
    if(detectActor)
    {
        this->GetDefaultRenderer()->RemoveActor(detectActor);
        detectActor = nullptr;
    }

    if(pickedActor)
    {
        vtkNew<vtkOutlineSource> outlineSource;
        outlineSource->SetBounds(pickedActor->GetBounds());
        vtkNew<vtkPolyDataMapper> outlineMapper;
        outlineMapper->SetInputConnection(outlineSource->GetOutputPort());
        detectActor = vtkSmartPointer<vtkActor>::New();
        detectActor->SetMapper(outlineMapper);
        detectActor->GetProperty()->SetColor(1.0,0.0,0.0);
        detectActor->GetProperty()->SetLineWidth(2);
        this->GetDefaultRenderer()->AddActor(detectActor);
    }

    this->GetDefaultRenderer()->GetRenderWindow()->Render();
}

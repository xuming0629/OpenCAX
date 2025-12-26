#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"
#include "SARibbonApplicationButton.h"
#include "SARibbonMenu.h"

#include <QDockWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>

/* ================= 构造 / 析构 ================= */

MainWindow::MainWindow(QWidget *parent)
    : SARibbonMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化 Ribbon
    initRibbon();

    // 初始化 Dock（左树 + 底部日志）
    initDockWidgets();

    // 初始化中央 3D 可视化
    initCentralVTKView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* ================= 中央 VTK ================= */

// void MainWindow::initCentralVTKView()
// {
//     // 创建 VTK Widget
//     wid_vtk = new MVtkWidget(this);

//     // 外层 Frame（背景 & 边框）
//     vtkFrame = new QFrame(this);
//     vtkFrame->setObjectName("vtkFrame");
//     vtkFrame->setFrameShape(QFrame::StyledPanel);
//     vtkFrame->setFrameShadow(QFrame::Plain);
//     vtkFrame->setLineWidth(2);
//     // vtkFrame->setStyleSheet(
//     //     "QFrame#vtkFrame { background-color: #f0f0f0; border: 2px solid #2a82da; }"
//     //     );

//     // 布局
//     QVBoxLayout* layout = new QVBoxLayout(vtkFrame);
//     layout->setContentsMargins(4,0,4,0);
//     layout->setSpacing(4);
//     layout->addWidget(wid_vtk);

//     // 设置为中央窗口
//     setCentralWidget(vtkFrame);
// }
void MainWindow::initCentralVTKView()
{
    // 创建 VTK Widget
    wid_vtk = new MVtkWidget(this);

    // 外层 Frame（背景 & 边框）
    vtkFrame = new QFrame(this);
    vtkFrame->setObjectName("vtkFrame");
    vtkFrame->setFrameShape(QFrame::StyledPanel);
    vtkFrame->setFrameShadow(QFrame::Plain);
    vtkFrame->setLineWidth(2);
    // vtkFrame->setStyleSheet(
    //     "QFrame#vtkFrame { background-color: #f0f0f0; border: 2px solid #2a82da; border-radius: 4px; }"
    //     );

    // 内层容器，增加左右上下留白
    QWidget* innerWidget = new QWidget(vtkFrame);
    QVBoxLayout* innerLayout = new QVBoxLayout(innerWidget);
    innerLayout->setContentsMargins(4,4,4,4); // 上下左右留白
    innerLayout->setSpacing(0);
    innerLayout->addWidget(wid_vtk);

    // 外层 Frame 布局
    QVBoxLayout* layout = new QVBoxLayout(vtkFrame);
    layout->setContentsMargins(2,2,2,2); // Frame 内边距
    layout->setSpacing(0);
    layout->addWidget(innerWidget);

    // 设置为中央窗口
    setCentralWidget(vtkFrame);
}

/* ================= Dock Widgets ================= */

// void MainWindow::initDockWidgets()
// {
//     // 左侧模型树
//     dockTree = new QDockWidget(tr("模型树"), this);
//     dockTree->setObjectName("dockTree");
//     dockTree->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

//     treeWidget = new QTreeWidget(dockTree);
//     treeWidget->setHeaderLabel("Model Explorer");
//     treeWidget->setFrameShape(QFrame::StyledPanel);
//     // treeWidget->setStyleSheet(
//     //     "QTreeWidget { border: 2px solid #2a82da; background-color: #ffffff; }"
//     //     );

//     QTreeWidgetItem* rootGeom = new QTreeWidgetItem(treeWidget);
//     rootGeom->setText(0, "Geometry");

//     QTreeWidgetItem* rootMesh = new QTreeWidgetItem(treeWidget);
//     rootMesh->setText(0, "Mesh");

//     dockTree->setWidget(treeWidget);
//     addDockWidget(Qt::LeftDockWidgetArea, dockTree);

//     // 底部日志
//     dockLog = new QDockWidget(tr("日志"), this);
//     dockLog->setObjectName("dockLog");
//     dockLog->setAllowedAreas(Qt::BottomDockWidgetArea);

//     logWidget = new QTextEdit(dockLog);
//     logWidget->setReadOnly(true);
//     logWidget->setFrameShape(QFrame::StyledPanel);
//     // logWidget->setStyleSheet(
//     //     "QTextEdit { border: 2px solid #2a82da; background-color: #fafafa; }"
//     //     );
//     logWidget->append("[INFO] OpenFinite started.");

//     dockLog->setWidget(logWidget);
//     addDockWidget(Qt::BottomDockWidgetArea, dockLog);
// }
void MainWindow::initDockWidgets()
{
    // 左侧模型树
    dockTree = new QDockWidget(tr("模型树"), this);
    dockTree->setObjectName("dockTree");
    dockTree->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // 外层背景容器，用于显示工业灰
    QWidget* treeBackground = new QWidget(dockTree);
    treeBackground->setStyleSheet("background-color: #D0D0D0;"); // 工业灰
    QVBoxLayout* bgLayout = new QVBoxLayout(treeBackground);
    bgLayout->setContentsMargins(4,4,4,4);  // 外边距显示背景色
    bgLayout->setSpacing(0);

    // 内层容器放实际控件
    QWidget* treeContainer = new QWidget(treeBackground);
    QVBoxLayout* treeLayout = new QVBoxLayout(treeContainer);
    treeLayout->setContentsMargins(0,0,0,0); // 内部 Tree 紧贴容器
    treeLayout->setSpacing(0);

    treeWidget = new QTreeWidget(treeContainer);
    treeWidget->setHeaderLabel("Model Explorer");
    treeWidget->setFrameShape(QFrame::StyledPanel);  // 保留边框
    treeLayout->addWidget(treeWidget);

    // 根节点
    QTreeWidgetItem* rootGeom = new QTreeWidgetItem(treeWidget);
    rootGeom->setText(0, "Geometry");
    QTreeWidgetItem* rootMesh = new QTreeWidgetItem(treeWidget);
    rootMesh->setText(0, "Mesh");

    bgLayout->addWidget(treeContainer);  // 内容器放在背景上
    dockTree->setWidget(treeBackground);
    addDockWidget(Qt::LeftDockWidgetArea, dockTree);

    // 底部日志
    dockLog = new QDockWidget(tr("日志"), this);
    dockLog->setObjectName("dockLog");
    dockLog->setAllowedAreas(Qt::BottomDockWidgetArea);

    QWidget* logBackground = new QWidget(dockLog);
    logBackground->setStyleSheet("background-color: #D0D0D0;"); // 工业灰
    QVBoxLayout* logBgLayout = new QVBoxLayout(logBackground);
    logBgLayout->setContentsMargins(4,4,4,4);
    logBgLayout->setSpacing(0);

    QWidget* logContainer = new QWidget(logBackground);
    QVBoxLayout* logLayout = new QVBoxLayout(logContainer);
    logLayout->setContentsMargins(0,0,0,0);  // 内部 QTextEdit 紧贴容器
    logLayout->setSpacing(0);

    logWidget = new QTextEdit(logContainer);
    logWidget->setReadOnly(true);
    logWidget->setFrameShape(QFrame::StyledPanel);
    logLayout->addWidget(logWidget);
    logWidget->append("[INFO] OpenFinite started.");

    logBgLayout->addWidget(logContainer);
    dockLog->setWidget(logBackground);
    addDockWidget(Qt::BottomDockWidgetArea, dockLog);
}




/* ================= Ribbon ================= */

void MainWindow::initRibbon()
{
    setWindowTitle("OpenFinite");
    setWindowModified(true);

    ribbon = new SARibbonBar(this);
    setContentsMargins(1,0,1,0);
    ribbon->setContentsMargins(4,0,4,0);
    ribbon->applicationButton()->hide();

    // 挂到 MainWindow
    setRibbonBar(ribbon);

    // 设置 Ribbon 风格主题
    setRibbonTheme(SARibbonTheme::RibbonThemeOffice2016Blue);

    // 给 Ribbon Panel 添加底部分割线
    // ribbon->setStyleSheet(
    //     "SARibbonPanel { border-bottom: 2px solid #2a82da; padding-bottom: 4px; }"
    //     "SARibbonPanel QToolButton { border-bottom: 1px solid #2a82da; padding: 5px; }"
    //     );



    // 创建各模块 Ribbon Page
    createFilePage();
    createViewPage();
    createGeometryPage();
    createMeshPage();
    createSolvePage();
    createPostPage();
}

/* ================= Ribbon Page：文件 ================= */

void MainWindow::createFilePage()
{
    SARibbonCategory* fileCategory = ribbon->addCategoryPage(QStringLiteral("文件"));
    SARibbonPanel* panel = fileCategory->addPanel(QStringLiteral("文件操作"));

    QAction* actImport  = new QAction(QIcon(":/imgs/icons/3dModel.png"), "导入模型", this);
    QAction* actExport  = new QAction(QIcon(":/imgs/icons/3dModel.png"), "导出模型", this);
    QAction* actSave    = new QAction(QIcon(":/imgs/icons/save.png"),   "保存", this);

    panel->addLargeAction(actImport);
    panel->addLargeAction(actExport);
    panel->addLargeAction(actSave);

    connect(actImport, &QAction::triggered, this, &MainWindow::onImportModel);
    connect(actExport, &QAction::triggered, this, &MainWindow::onExportModel);
    // connect(actSave, &QAction::triggered, this, &MainWindow::onSaveProject);
}

/* ================= Ribbon Page：视图 ================= */

void MainWindow::createViewPage()
{
    SARibbonCategory* viewCategory = ribbon->addCategoryPage(QStringLiteral("视图"));
    SARibbonPanel* panel = viewCategory->addPanel(QStringLiteral("显示"));

    QAction* actFitAll = new QAction(QIcon(":/imgs/icon/axis.png"), "缩放到全部", this);
    QAction* actReset  = new QAction(QIcon(":/imgs/icon/axis.png"), "重置视图", this);
    QAction* actFront  = new QAction(QIcon(":/imgs/icon/axis.png"), "前视图", this);
    QAction* actBack   = new QAction(QIcon(":/imgs/icon/axis.png"), "后视图", this);
    QAction* actLeft   = new QAction(QIcon(":/imgs/icon/axis.png"), "左视图", this);
    QAction* actRight  = new QAction(QIcon(":/imgs/icon/axis.png"), "右视图", this);

    panel->addLargeAction(actFitAll);
    panel->addLargeAction(actReset);
    panel->addLargeAction(actFront);
    panel->addLargeAction(actBack);
    panel->addLargeAction(actLeft);
    panel->addLargeAction(actRight);

    connect(actFitAll, &QAction::triggered, this, [this](){ logWidget->append("[VIEW] 缩放到全部"); });
    connect(actReset, &QAction::triggered, this, [this](){ logWidget->append("[VIEW] 重置视图"); });
    connect(actFront, &QAction::triggered, this, [this](){ logWidget->append("[VIEW] 前视图"); });
    connect(actBack, &QAction::triggered, this, [this](){ logWidget->append("[VIEW] 后视图"); });
    connect(actLeft, &QAction::triggered, this, [this](){ logWidget->append("[VIEW] 左视图"); });
    connect(actRight, &QAction::triggered, this, [this](){ logWidget->append("[VIEW] 右视图"); });
}

/* ================= Ribbon Page：几何 ================= */

void MainWindow::createGeometryPage()
{
    SARibbonCategory* geomCategory = ribbon->addCategoryPage(QStringLiteral("几何"));

    SARibbonPanel* pPoint = geomCategory->addPanel(QStringLiteral("点"));
    QAction* actPoint = new QAction("创建点", this);
    pPoint->addLargeAction(actPoint);
    connect(actPoint, &QAction::triggered, this, [this](){
        logWidget->append("[GEOM] 创建点");
    });

    SARibbonPanel* pEdge = geomCategory->addPanel(QStringLiteral("线"));
    QAction* actLine = new QAction("直线", this);
    QAction* actArc  = new QAction("圆弧", this);
    pEdge->addLargeAction(actLine);
    pEdge->addLargeAction(actArc);
    connect(actLine, &QAction::triggered, this, [this](){ logWidget->append("[GEOM] 创建直线"); });
}

/* ================= Ribbon Page：网格 ================= */

void MainWindow::createMeshPage()
{
    SARibbonCategory* meshCategory = ribbon->addCategoryPage(QStringLiteral("网格"));

    SARibbonPanel* pMeshGen = meshCategory->addPanel(QStringLiteral("网格生成"));
    QAction* actGenMesh = new QAction("生成网格", this);
    pMeshGen->addLargeAction(actGenMesh);
    connect(actGenMesh, &QAction::triggered, this, [this](){ logWidget->append("[MESH] 生成网格"); });
}

/* ================= Ribbon Page：求解 ================= */

void MainWindow::createSolvePage()
{
    SARibbonCategory* solveCategory = ribbon->addCategoryPage(QStringLiteral("求解"));
    SARibbonPanel* pSolve = solveCategory->addPanel(QStringLiteral("求解操作"));
    QAction* actSolve = new QAction("开始求解", this);
    pSolve->addLargeAction(actSolve);
    connect(actSolve, &QAction::triggered, this, [this](){ logWidget->append("[SOLVE] 求解开始"); });
}

/* ================= Ribbon Page：后处理 ================= */

void MainWindow::createPostPage()
{
    SARibbonCategory* postCategory = ribbon->addCategoryPage(QStringLiteral("后处理"));
    SARibbonPanel* pPost = postCategory->addPanel(QStringLiteral("结果分析"));

    QAction* actViewResult = new QAction("查看结果", this);
    pPost->addLargeAction(actViewResult);
    connect(actViewResult, &QAction::triggered, this, [this](){ logWidget->append("[POST] 查看计算结果"); });
}

/* ================= 文件操作 ================= */

void MainWindow::onImportModel()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "选择模型文件", "", "STEP/IGES Files (*.step *.stp *.iges *.igs);;All Files (*)");
    if (fileName.isEmpty()) return;

    logWidget->append("[FILE] 导入文件: " + fileName);

    TopoDS_Shape shape;
    if (!modelManager.importModel(fileName, shape)) {
        logWidget->append("[ERROR] 导入失败: " + fileName);
        return;
    }

    // 直接显示
    wid_vtk->disPlayOCC(shape);
    logWidget->append("[INFO] 模型导入并显示成功");
}

void MainWindow::onExportModel()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "保存模型文件", "", "STEP Files (*.step);;IGES Files (*.iges)");
    if (fileName.isEmpty()) return;

    // 使用 MVtkWidget 内部保存的 currentShape
    if (wid_vtk->currentShape.IsNull())
    {
        logWidget->append("[ERROR] 没有模型可导出");
        return;
    }

    if (!modelManager.exportModel(wid_vtk->currentShape, fileName))
    {
        logWidget->append("[ERROR] 导出失败: " + fileName);
        return;
    }

    logWidget->append("[INFO] 导出成功: " + fileName);
}

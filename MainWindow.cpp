#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"

#include <QDockWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QAction>
#include <QDebug>

/* ================= 构造 / 析构 ================= */

MainWindow::MainWindow(QWidget *parent)
    : SARibbonMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化 Ribbon + 页面
    initUI();

    // 初始化 Dock（左树 + 底部日志）
    initDockWidgets();

    // 中央 3D 可视化窗口
    wid_vtk = new MVtkWidget(this);
    setCentralWidget(wid_vtk);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* ================= UI 初始化 ================= */

void MainWindow::initUI()
{
    setWindowTitle("OpenFinite");
    setWindowModified(true);

    // 创建 Ribbon
    ribbon = new SARibbonBar(this);
    setContentsMargins(1, 0, 1, 0);
    ribbon->setContentsMargins(0, 0, 0, 0);
    setRibbonBar(ribbon);

    /* -------- 文件 -------- */
    SARibbonCategory* fileCategory =
        ribbon->addCategoryPage(QStringLiteral("文件"));
    createCategoryFile(fileCategory);

    /* -------- 几何 -------- */
    SARibbonCategory* geomCategory =
        ribbon->addCategoryPage(QStringLiteral("几何"));
    createCategoryGeometry(geomCategory);

    /* -------- 网格 -------- */
    SARibbonCategory* meshCategory =
        ribbon->addCategoryPage(QStringLiteral("网格"));
    createCategoryMesh(meshCategory);
}

/* ================= Dock Widgets ================= */

void MainWindow::initDockWidgets()
{
    /* ========== 左侧：模型树 ========== */
    dockTree = new QDockWidget(tr("模型树"), this);
    dockTree->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    treeWidget = new QTreeWidget(dockTree);
    treeWidget->setHeaderLabel("Model Explorer");

    // 示例节点
    QTreeWidgetItem* rootGeom = new QTreeWidgetItem(treeWidget);
    rootGeom->setText(0, "Geometry");

    QTreeWidgetItem* rootMesh = new QTreeWidgetItem(treeWidget);
    rootMesh->setText(0, "Mesh");

    dockTree->setWidget(treeWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockTree);

    /* ========== 底部：日志窗口 ========== */
    dockLog = new QDockWidget(tr("日志"), this);
    dockLog->setAllowedAreas(Qt::BottomDockWidgetArea);

    logWidget = new QTextEdit(dockLog);
    logWidget->setReadOnly(true);
    logWidget->append("[INFO] OpenFinite started.");

    dockLog->setWidget(logWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dockLog);
}

/* ================= Ribbon：文件 ================= */

void MainWindow::createCategoryFile(SARibbonCategory* page)
{
    SARibbonPanel* panelFile = page->addPanel(QStringLiteral("文件"));

    QAction* actNew  = new QAction("新建", this);
    QAction* actOpen = new QAction("打开", this);
    QAction* actSave = new QAction("保存", this);

    panelFile->addLargeAction(actNew);
    panelFile->addLargeAction(actOpen);
    panelFile->addLargeAction(actSave);

    connect(actNew, &QAction::triggered, this, [this]() {
        logWidget->append("[INFO] 新建工程");
    });
}

/* ================= Ribbon：几何 ================= */

void MainWindow::createCategoryGeometry(SARibbonCategory* page)
{
    /* ---------- 点 ---------- */
    SARibbonPanel* pPoint = page->addPanel(QStringLiteral("点"));

    QAction* actPoint = new QAction("创建点", this);
    pPoint->addLargeAction(actPoint);

    connect(actPoint, &QAction::triggered, this, [this]() {
        logWidget->append("[GEOM] 创建点");
        qDebug() << "Create Point";
        // TODO: OCC 建点 + 加入树
    });

    /* ---------- 线 ---------- */
    SARibbonPanel* pEdge = page->addPanel(QStringLiteral("线"));

    QAction* actLine = new QAction("直线", this);
    QAction* actArc  = new QAction("圆弧", this);

    pEdge->addLargeAction(actLine);
    pEdge->addLargeAction(actArc);

    connect(actLine, &QAction::triggered, this, [this]() {
        logWidget->append("[GEOM] 创建直线");
    });
}

/* ================= Ribbon：网格 ================= */

void MainWindow::createCategoryMesh(SARibbonCategory* page)
{
    /* ---------- 网格生成 ---------- */
    SARibbonPanel* pMeshGen = page->addPanel(QStringLiteral("网格"));

    QAction* actGenMesh = new QAction("生成网格", this);
    pMeshGen->addLargeAction(actGenMesh);

    connect(actGenMesh, &QAction::triggered, this, [this]() {
        logWidget->append("[MESH] 生成网格");
        qDebug() << "Generate Mesh";
        // TODO: Gmsh / OCC Mesh
    });

    /* ---------- 求解 ---------- */
    SARibbonPanel* pSolve = page->addPanel(QStringLiteral("求解"));

    QAction* actSolve = new QAction("开始求解", this);
    pSolve->addLargeAction(actSolve);

    connect(actSolve, &QAction::triggered, this, [this]() {
        logWidget->append("[SOLVE] 求解开始");
    });
}

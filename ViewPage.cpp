#include "ViewPage.h"
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"
#include "QAction"
#include "MVtkWidget.h"
#include "QTextEdit"

ViewPage::ViewPage(SARibbonBar* ribbon, MVtkWidget* vtk, QTextEdit* log, QObject* parent)
    : QObject(parent)
    , mRibbon(ribbon)
    , mVtk(vtk)
    , mLog(log)
{
}

void ViewPage::create()
{
    createActions();
    createUI();
    connectSignals();
}

void ViewPage::createActions()
{
    // 缩放到全部
    actFitAll = new QAction("缩放到全部", this);

    // AXO 显示
    actAXO = new QAction("轴测视图", this);

    // XYZ 方向
    actPosX = new QAction("正X", this);
    actNegX = new QAction("负X", this);
    actPosY = new QAction("正Y", this);
    actNegY = new QAction("负Y", this);
    actPosZ = new QAction("正Z", this);
    actNegZ = new QAction("负Z", this);

    // 便捷视图
    // actTop    = new QAction("上视图", this);
    // actBottom = new QAction("下视图", this);
    // actLeft   = new QAction("左视图", this);
    // actRight  = new QAction("右视图", this);
    // actFront  = new QAction("前视图", this);
    // actBack   = new QAction("后视图", this);
}

void ViewPage::createUI()
{
    if (!mRibbon) return;

    mCategory = mRibbon->addCategoryPage("视图");
    SARibbonPanel* panel = mCategory->addPanel("显示");

    // 添加按钮到 Panel
    panel->addLargeAction(actFitAll);
    panel->addLargeAction(actAXO);
    // panel->addLargeAction(actTop);
    // panel->addLargeAction(actBottom);
    // panel->addLargeAction(actLeft);
    // panel->addLargeAction(actRight);
    // panel->addLargeAction(actFront);
    // panel->addLargeAction(actBack);
}

void ViewPage::connectSignals()
{
    if (!mVtk || !mLog) return;

    connect(actFitAll, &QAction::triggered, this, [this]() {
        mVtk->fitAll();
        mLog->append("[VIEW] 缩放到全部");
    });

    connect(actAXO, &QAction::triggered, this, [this]() {
        mVtk->ViewAXO();
        mLog->append("[VIEW] 轴测视图");
    });

    // connect(actTop, &QAction::triggered, this, [this]() {
    //     mVtk->ViewTop();
    //     mLog->append("[VIEW] 上视图");
    // });

    // connect(actBottom, &QAction::triggered, this, [this]() {
    //     mVtk->ViewBottom();
    //     mLog->append("[VIEW] 下视图");
    // });

    // connect(actLeft, &QAction::triggered, this, [this]() {
    //     mVtk->ViewLeft();
    //     mLog->append("[VIEW] 左视图");
    // });

    // connect(actRight, &QAction::triggered, this, [this]() {
    //     mVtk->ViewRight();
    //     mLog->append("[VIEW] 右视图");
    // });

    // connect(actFront, &QAction::triggered, this, [this]() {
    //     mVtk->ViewFront();
    //     mLog->append("[VIEW] 前视图");
    // });

    // connect(actBack, &QAction::triggered, this, [this]() {
    //     mVtk->ViewBack();
    //     mLog->append("[VIEW] 后视图");
    // });
}

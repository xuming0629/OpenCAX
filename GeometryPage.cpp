#include "GeometryPage.h"
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"
#include <QAction>
#include <QTextEdit>
GeometryPage::GeometryPage(SARibbonBar* ribbon, QTextEdit* log, QObject* parent)
    : QObject(parent), mRibbon(ribbon), mLog(log)
{
}

void GeometryPage::create()
{
    createActions();
    createUI();
    connectSignals();
}

void GeometryPage::createActions()
{
    actCreatePoint = new QAction("创建点", this);
    actCreateLine  = new QAction("直线", this);
    actCreateArc   = new QAction("圆弧", this);
}

void GeometryPage::createUI()
{
    mCategory = mRibbon->addCategoryPage("几何");

    SARibbonPanel* pPoint = mCategory->addPanel("点");
    pPoint->addLargeAction(actCreatePoint);

    SARibbonPanel* pEdge = mCategory->addPanel("线");
    pEdge->addLargeAction(actCreateLine);
    pEdge->addLargeAction(actCreateArc);
}

void GeometryPage::connectSignals()
{
    connect(actCreatePoint, &QAction::triggered, this, [this](){
        if(mLog) mLog->append("[GEOM] 创建点");
    });
    connect(actCreateLine, &QAction::triggered, this, [this](){
        if(mLog) mLog->append("[GEOM] 创建直线");
    });
    connect(actCreateArc, &QAction::triggered, this, [this](){
        if(mLog) mLog->append("[GEOM] 创建圆弧");
    });
}

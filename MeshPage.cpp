#include "MeshPage.h"
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"
#include <QAction>
#include <QTextEdit>

MeshPage::MeshPage(SARibbonBar* ribbon, QTextEdit* log, QObject* parent)
    : QObject(parent), mRibbon(ribbon), mLog(log)
{
}

void MeshPage::create()
{
    createActions();
    createUI();
    connectSignals();
}

void MeshPage::createActions()
{
    actGenerateMesh = new QAction("生成网格", this);
}

void MeshPage::createUI()
{
    mCategory = mRibbon->addCategoryPage("网格");

    SARibbonPanel* pPanel = mCategory->addPanel("网格生成");
    pPanel->addLargeAction(actGenerateMesh);
}

void MeshPage::connectSignals()
{
    connect(actGenerateMesh, &QAction::triggered, this, [this](){
        if(mLog) mLog->append("[MESH] 生成网格");
    });
}

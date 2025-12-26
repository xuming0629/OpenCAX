#include "PostPage.h"
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"
#include <QAction>
#include <QTextEdit>

PostPage::PostPage(SARibbonBar* ribbon, QTextEdit* log, QObject* parent)
    : QObject(parent), mRibbon(ribbon), mLog(log)
{
}

void PostPage::create()
{
    createActions();
    createUI();
    connectSignals();
}

void PostPage::createActions()
{
    actViewResult = new QAction("查看结果", this);
}

void PostPage::createUI()
{
    mCategory = mRibbon->addCategoryPage("后处理");

    SARibbonPanel* pPanel = mCategory->addPanel("结果分析");
    pPanel->addLargeAction(actViewResult);
}

void PostPage::connectSignals()
{
    connect(actViewResult, &QAction::triggered, this, [this](){
        if(mLog) mLog->append("[POST] 查看计算结果");
    });
}

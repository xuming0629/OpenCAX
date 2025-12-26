#include "SolvePage.h"
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"
#include <QAction>
#include <QTextEdit>

SolvePage::SolvePage(SARibbonBar* ribbon, QTextEdit* log, QObject* parent)
    : QObject(parent), mRibbon(ribbon), mLog(log)
{
}

void SolvePage::create()
{
    createActions();
    createUI();
    connectSignals();
}

void SolvePage::createActions()
{
    actSolve = new QAction("开始求解", this);
}

void SolvePage::createUI()
{
    mCategory = mRibbon->addCategoryPage("求解");

    SARibbonPanel* pPanel = mCategory->addPanel("求解操作");
    pPanel->addLargeAction(actSolve);
}

void SolvePage::connectSignals()
{
    connect(actSolve, &QAction::triggered, this, [this](){
        if(mLog) mLog->append("[SOLVE] 求解开始");
    });
}

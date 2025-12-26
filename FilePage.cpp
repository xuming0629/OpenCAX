#include "FilePage.h"

#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPanel.h"

#include <QAction>
#include <QFileDialog>
#include <QTextEdit>

#include "MVtkWidget.h"
#include "ModelManager.h"

// OpenCASCADE
#include <TopoDS_Shape.hxx>

FilePage::FilePage(SARibbonBar* ribbon,
                   MVtkWidget* vtk,
                   ModelManager* model,
                   QTextEdit* log,
                   QObject* parent)
    : QObject(parent)
    , mRibbon(ribbon)
    , mVtk(vtk)
    , mModel(model)
    , mLog(log)
{
}

void FilePage::create()
{
    // ===== Category =====
    mCategory = mRibbon->addCategoryPage(tr("文件"));

    // ===== Panel =====
    SARibbonPanel* panelIO = mCategory->addPanel(tr("模型"));

    // ===== Actions =====
    QAction* actImport  = new QAction(QIcon(":/imgs/icons/3dModel.png"), "导入模型", this);
    QAction* actExport  = new QAction(QIcon(":/imgs/icons/3dModel.png"), "导出模型", this);

    panelIO->addLargeAction(actImport);
    panelIO->addLargeAction(actExport);

    // ===== 信号槽（自治）=====
    connect(actImport, &QAction::triggered,
            this, &FilePage::importModel);

    // connect(actExport, &QAction::triggered,
    //         this, &FilePage::exportModel);
}

void FilePage::importModel()
{
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        tr("导入模型"),
        QString(),
        tr("CAD Files (*.step *.stp *.iges *.igs)")
        );

    if (fileName.isEmpty())
        return;

    TopoDS_Shape shape;
    if (!mModel->importModel(fileName, shape))
    {
        if (mLog)
            mLog->append("[ERROR] 模型导入失败");
        return;
    }

    // 显示到 VTK（你之前已经说可以直接用 disPlayOCC）
    if (mVtk)
        mVtk->disPlayOCC(shape);

    if (mLog)
        mLog->append("[FILE] 模型导入成功: " + fileName);
}

// void FilePage::exportModel()
// {
//     QString fileName = QFileDialog::getSaveFileName(
//         nullptr,
//         tr("导出模型"),
//         QString(),
//         tr("STEP (*.step *.stp)")
//         );

//     if (fileName.isEmpty())
//         return;

//     if (!mModel->exportModel(fileName))
//     {
//         if (mLog)
//             mLog->append("[ERROR] 模型导出失败");
//         return;
//     }

//     if (mLog)
//         mLog->append("[FILE] 模型导出成功: " + fileName);
// }

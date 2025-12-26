#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QObject>

class SARibbonBar;
class SARibbonCategory;
class SARibbonPanel;
class QAction;
class QTextEdit;

class MVtkWidget;
class ModelManager;

class FilePage : public QObject
{
    Q_OBJECT
public:
    explicit FilePage(SARibbonBar* ribbon,
                      MVtkWidget* vtk,
                      ModelManager* model,
                      QTextEdit* log,
                      QObject* parent = nullptr);

    void create();   // 创建 Ribbon Page

private slots:
    void importModel();
    // void exportModel();

private:
    SARibbonBar*   mRibbon {nullptr};
    MVtkWidget*    mVtk    {nullptr};
    ModelManager*  mModel  {nullptr};
    QTextEdit*     mLog    {nullptr};

    SARibbonCategory* mCategory {nullptr};
};

#endif // FILEPAGE_H

#ifndef VIEWPAGE_H
#define VIEWPAGE_H

#include <QObject>

class SARibbonBar;
class SARibbonCategory;
class SARibbonPanel;
class QAction;
class QTextEdit;
class MVtkWidget;

class ViewPage : public QObject
{
    Q_OBJECT
public:
    explicit ViewPage(
        SARibbonBar* ribbon,
        MVtkWidget* vtk,
        QTextEdit* log,
        QObject* parent = nullptr
        );

    void create();  // 创建 Ribbon 页面

private:
    void createActions();
    void createUI();
    void connectSignals();

private:
    SARibbonBar* mRibbon {nullptr};
    SARibbonCategory* mCategory {nullptr};
    MVtkWidget* mVtk {nullptr};
    QTextEdit* mLog {nullptr};

    // 显示操作
    QAction* actFitAll {nullptr};

    // 视图方向
    QAction* actAXO {nullptr};
    QAction* actPosX {nullptr};
    QAction* actNegX {nullptr};
    QAction* actPosY {nullptr};
    QAction* actNegY {nullptr};
    QAction* actPosZ {nullptr};
    QAction* actNegZ {nullptr};

    // 方便调用的前/后/左/右/上/下视图
    QAction* actTop {nullptr};
    QAction* actBottom {nullptr};
    QAction* actLeft {nullptr};
    QAction* actRight {nullptr};
    QAction* actFront {nullptr};
    QAction* actBack {nullptr};
};

#endif // VIEWPAGE_H

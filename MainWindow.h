// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include "SARibbonMainWindow.h"
// #include "MVTKWidget.h"
// class SARibbonCategory;
// class SARibbonPannel;
// class SARibbonBar;

// namespace Ui {
// class MainWindow;
// }

// class MainWindow : public SARibbonMainWindow
// {
//     Q_OBJECT

// public:
//     explicit MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private:
//     void initUI();

//     void createCategoryFile(SARibbonCategory* page);

//     // 创建几何模块 Ribbon 分类
//     void createCategoryGeometry(SARibbonCategory* page);

//     void createCategoryMesh(SARibbonCategory* page);

//     SARibbonBar *ribbon;


// private:
//     MVtkWidget* wid_vtk;

// private:
//     Ui::MainWindow *ui;
// };

// #endif // MAINWINDOW_H
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SARibbonMainWindow.h"
#include "MVTKWidget.h"

class SARibbonCategory;
class SARibbonBar;
class QDockWidget;
class QTreeWidget;
class QTextEdit;

namespace Ui {
class MainWindow;
}

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* ===== UI 初始化 ===== */
    void initUI();
    void initDockWidgets();

    /* ===== Ribbon 分类 ===== */
    void createCategoryFile(SARibbonCategory* page);
    void createCategoryGeometry(SARibbonCategory* page);
    void createCategoryMesh(SARibbonCategory* page);

private:
    SARibbonBar* ribbon {nullptr};

    /* ===== 中央 3D 视图 ===== */
    MVtkWidget* wid_vtk {nullptr};

    /* ===== Dock Widgets ===== */
    QDockWidget* dockTree {nullptr};
    QDockWidget* dockLog  {nullptr};

    QTreeWidget* treeWidget {nullptr};
    QTextEdit*  logWidget  {nullptr};

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

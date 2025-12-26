// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include "SARibbonMainWindow.h"
// #include "MVtkWidget.h"
// #include <QFrame>

// #include "ModelManager.h"
// class SARibbonCategory;
// class SARibbonBar;
// class QDockWidget;
// class QTreeWidget;
// class QTextEdit;
// class SARibbonMenu;

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
//     /* ===== Ribbon 初始化 ===== */
//     void initRibbon();


//     /* ===== Dock Widgets 初始化 ===== */
//     void initDockWidgets();

//     /* ===== Ribbon 页面创建 ===== */
//     void createFilePage();
//     void createViewPage();
//     void createGeometryPage();
//     void createMeshPage();
//     void createSolvePage();
//     void createPostPage();

//     void initCentralVTKView();

//     /* ===== 辅助函数 ===== */
//     QAction* createAction(const QString& text, const QString& iconPath);


// private slots:
//     void onImportModel();   // 导入模型
//     void onExportModel();   // 导出模型
//     // void onSaveProject();   // 保存工程




// private:
//     SARibbonBar* ribbon {nullptr};


//     /* ===== Dock Widgets ===== */
//     QDockWidget* dockTree {nullptr};
//     QDockWidget* dockLog  {nullptr};

//     QTreeWidget* treeWidget {nullptr};
//     QTextEdit*  logWidget  {nullptr};

//     /* ===== ApplicationButton 菜单 ===== */
//     SARibbonMenu* mMenuApplicationBtn {nullptr};

//     MVtkWidget* wid_vtk = nullptr;
//     QFrame*     vtkFrame = nullptr;

//     ModelManager modelManager;    // <-- 新增

// private:
//     Ui::MainWindow *ui;
// };

// #endif // MAINWINDOW_H


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SARibbonMainWindow.h"
#include "MVtkWidget.h"
#include "ModelManager.h"

// Ribbon Pages
// #include "FilePage.h"
// #include "ViewPage.h"
// 以后新增：
// #include "GeometryPage.h"
// #include "MeshPage.h"
// #include "SolvePage.h"
// #include "PostPage.h"

#include <QFrame>

class SARibbonBar;
class QDockWidget;
class QTreeWidget;
class QTextEdit;
class SARibbonMenu;

class FilePage;
class ViewPage;
class GeometryPage;
class MeshPage;
class SolvePage;
class PostPage;

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
    /* ===== 初始化 ===== */
    void initRibbon();
    void initDockWidgets();
    void initCentralVTKView();


private:
    /* ===== Ribbon ===== */
    SARibbonBar* ribbon {nullptr};

    /* ===== Ribbon Pages（只保存指针）===== */
    FilePage* filePage {nullptr};
    ViewPage* viewPage {nullptr};
    GeometryPage* geometryPage {nullptr};
    // MeshPage*     meshPage {nullptr};
    // SolvePage*    solvePage {nullptr};
    // PostPage*     postPage {nullptr};

    /* ===== Dock Widgets ===== */
    QDockWidget* dockTree {nullptr};
    QDockWidget* dockLog  {nullptr};

    QTreeWidget* treeWidget {nullptr};
    QTextEdit*  logWidget  {nullptr};

    /* ===== Central VTK ===== */
    MVtkWidget* wid_vtk  {nullptr};
    QFrame*     vtkFrame {nullptr};

    /* ===== Core ===== */
    ModelManager modelManager;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

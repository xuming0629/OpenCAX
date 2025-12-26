#ifndef MESHPAGE_H
#define MESHPAGE_H

#include <QObject>

class SARibbonBar;
class SARibbonCategory;
class SARibbonPanel;
class QAction;
class QTextEdit;

class MeshPage : public QObject
{
    Q_OBJECT
public:
    explicit MeshPage(SARibbonBar* ribbon, QTextEdit* log, QObject* parent = nullptr);
    void create();

private:
    void createActions();
    void createUI();
    void connectSignals();

private:
    SARibbonBar* mRibbon {nullptr};
    SARibbonCategory* mCategory {nullptr};
    QTextEdit* mLog {nullptr};

    QAction* actGenerateMesh {nullptr};
};

#endif // MESHPAGE_H

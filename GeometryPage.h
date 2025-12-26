#ifndef GEOMETRYPAGE_H
#define GEOMETRYPAGE_H

#include <QObject>

class SARibbonBar;
class SARibbonCategory;
class SARibbonPanel;
class QAction;
class QTextEdit;

class GeometryPage : public QObject
{
    Q_OBJECT
public:
    explicit GeometryPage(SARibbonBar* ribbon, QTextEdit* log, QObject* parent = nullptr);
    void create();

private:
    void createActions();
    void createUI();
    void connectSignals();

private:
    SARibbonBar* mRibbon {nullptr};
    SARibbonCategory* mCategory {nullptr};
    QTextEdit* mLog {nullptr};

    QAction* actCreatePoint {nullptr};
    QAction* actCreateLine {nullptr};
    QAction* actCreateArc {nullptr};
};

#endif // GEOMETRYPAGE_H

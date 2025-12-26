#ifndef SOLVEPAGE_H
#define SOLVEPAGE_H

#include <QObject>

class SARibbonBar;
class SARibbonCategory;
class SARibbonPanel;
class QAction;
class QTextEdit;

class SolvePage : public QObject
{
    Q_OBJECT
public:
    explicit SolvePage(SARibbonBar* ribbon, QTextEdit* log, QObject* parent = nullptr);
    void create();

private:
    void createActions();
    void createUI();
    void connectSignals();

private:
    SARibbonBar* mRibbon {nullptr};
    SARibbonCategory* mCategory {nullptr};
    QTextEdit* mLog {nullptr};

    QAction* actSolve {nullptr};
};

#endif // SOLVEPAGE_H

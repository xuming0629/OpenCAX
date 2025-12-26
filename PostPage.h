#ifndef POSTPAGE_H
#define POSTPAGE_H

#include <QObject>

class SARibbonBar;
class SARibbonCategory;
class SARibbonPanel;
class QAction;
class QTextEdit;

class PostPage : public QObject
{
    Q_OBJECT
public:
    explicit PostPage(SARibbonBar* ribbon, QTextEdit* log, QObject* parent = nullptr);
    void create();

private:
    void createActions();
    void createUI();
    void connectSignals();

private:
    SARibbonBar* mRibbon {nullptr};
    SARibbonCategory* mCategory {nullptr};
    QTextEdit* mLog {nullptr};

    QAction* actViewResult {nullptr};
};

#endif // POSTPAGE_H

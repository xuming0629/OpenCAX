#include "MainWindow.h"
#include <iostream>
#include <QApplication>
#include <QElapsedTimer>
#include "SARibbonBar.h"


int main(int argc, char *argv[])
{
    SARibbonBar::initHighDpi();

    QApplication a(argc, argv);

    QFont f = a.font();
    f.setFamily(u8"微软雅黑");
    a.setFont(f);

    QElapsedTimer cost;
    cost.start();

    MainWindow w;
    w.showMaximized();   // ⭐ 默认最大化

    qDebug() << "window build cost:" << cost.elapsed() << " ms";
    return a.exec();
}


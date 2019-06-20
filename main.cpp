#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    if (argc >= 2) {
        MainWindow w(nullptr, QApplication::arguments()[1]);
        w.show();
        return a.exec();
    } else {
        MainWindow w;
        w.show();
        return a.exec();
    }


}

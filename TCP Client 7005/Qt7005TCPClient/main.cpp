#include "mainwindow.h"
#include "clientconnector.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ClientConnector cManager;
    cManager.Test();

    return a.exec();
}

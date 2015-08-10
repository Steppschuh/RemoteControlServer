#include "mainwindow.h"
#include "trayicon.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Steppschuh");
    QApplication::setApplicationName("Remote Control Server");
    MainWindow w;

    return a.exec();
}

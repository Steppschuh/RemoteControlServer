#include "mainwindow.h"
#include "trayicon.h"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("RemoteControlServer"); // the name of the applications
    QApplication::setOrganizationName("Steppschuh");
    QApplication::setOrganizationDomain("net.lekotsch");     // your organisation bundleID as it is set somewhere on your mac / in xcode

    QMainWindow window;
    MainWindow w(&window);

    return a.exec();
}

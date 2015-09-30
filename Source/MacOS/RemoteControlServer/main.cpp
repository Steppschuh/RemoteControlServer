#include "mainwindow.h"
#include "runguard.h"
#include "trayicon.h"

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RunGuard guard( "remote_control_server" );
    if ( !guard.tryToRun() ){
        QMessageBox::warning(0, "Error", "There is already an instance of the Remote Control Server running. Use this instance instead or quit it before you start a new instance!");
        return 0;
    }

    QApplication::setApplicationName("RemoteControlServer");
    QApplication::setOrganizationName("Steppschuh");
    QApplication::setOrganizationDomain("com.steppschuh");

    QLoggingCategory::setFilterRules("qt.network.ssl.w arning=false");

    QMainWindow window;
    MainWindow w(&window);

    return a.exec();
}

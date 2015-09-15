#include "mainwindow.h"
#include "runguard.h"
#include "trayicon.h"

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RunGuard guard( "remote_control_server" );
    if ( !guard.tryToRun() ){
        QMessageBox::warning(0, "Error", "There is already an instance of the Remote Control Server running. Use this instance instead or quit it before you start a new instance!");
        return 0;
    }

    QApplication::setApplicationName("RemoteControlServer"); // the name of the applications
    QApplication::setOrganizationName("Steppschuh");
    QApplication::setOrganizationDomain("com.steppschuh");     // your organisation bundleID as it is set somewhere on your mac / in xcode

    QMainWindow window;
    MainWindow w(&window);

    return a.exec();
}

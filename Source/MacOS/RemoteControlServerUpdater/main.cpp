#include "updater.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Updater w;
    w.setWindowTitle("Remote Control Server Updater");
    w.show();

    return a.exec();
}

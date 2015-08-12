#include <QCoreApplication>
#include "updater.h"

#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Updater *u = new Updater();

    QObject::connect(u, SIGNAL(finished()), &a, SLOT(quit()));

    QTimer::singleShot(0, u, SLOT(run()));

    return a.exec();
}

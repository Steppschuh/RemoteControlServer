#include <QCoreApplication>
#include "updater.h"

#include <QTimer>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("qt.network.ssl.w arning=false");

    QCoreApplication a(argc, argv);
    Updater *u = new Updater();

    QObject::connect(u, SIGNAL(finished()), &a, SLOT(quit()));

    QTimer::singleShot(0, u, SLOT(run()));

    return a.exec();
}

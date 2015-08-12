#include <QtCore>
#include <QCoreApplication>
#include <QObject>

#include "updatetask.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UpdateTask *updateTask = new UpdateTask();

    QObject::connect(updateTask, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, updateTask, SLOT(run()));

    return a.exec();
}

#include "updatetask.h"

#include "unistd.h"

#include <QDebug>

UpdateTask::UpdateTask()
{

}

void UpdateTask::run()
{
    // Do processing here
    for (int i = 0; i < 10; ++i)
    {
        usleep(1000000);
        qDebug() << i;
    }

    emit finished();
}

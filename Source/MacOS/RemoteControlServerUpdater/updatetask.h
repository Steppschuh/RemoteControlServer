#ifndef UPDATETASK_H
#define UPDATETASK_H

#include <QObject>


class UpdateTask : public QObject
{
    Q_OBJECT
public:
    UpdateTask();

public slots:
    void run();

signals:
    void finished();
};

#endif // UPDATETASK_H

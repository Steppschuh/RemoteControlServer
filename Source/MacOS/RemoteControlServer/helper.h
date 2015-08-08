#ifndef HELPER_H
#define HELPER_H

#include <QString>

class Helper
{
public:
    static Helper *Instance();

    QString generateHelpMail();
    bool sendMail(QString address, QString subject, QString body);

private:
    Helper();
    static Helper *instance;
};

#endif // HELPER_H

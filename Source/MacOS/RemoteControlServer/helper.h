#ifndef HELPER_H
#define HELPER_H

#include <QString>

class Helper
{
public:
    Helper *Instance();

private:
    Helper();
    Helper *instance;

//    QString generateHelpMail();
//    bool sendMail(QString address, QString subject, QString body);
};

#endif // HELPER_H

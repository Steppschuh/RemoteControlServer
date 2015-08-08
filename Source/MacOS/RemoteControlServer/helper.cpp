#include "helper.h"

Helper *Helper::instance = NULL;

Helper *Helper::Instance()
{
    if (!instance)
    {
        instance = new Helper();
    }
    return instance;
}

Helper::Helper()
{

}

QString Helper::generateHelpMail()
{
    return "";
}

bool Helper::sendMail(QString address, QString subject, QString body)
{
    return false;
}


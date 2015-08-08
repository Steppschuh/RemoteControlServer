#include "helper.h"
#include "network.h"
#include "server.h"

#include <QDesktopServices>

#include <QDebug>

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
    App *app = Server::Instance()->getCurrentApp();

    QString temp = "Hey,\nI'm using ";
    temp = temp + ((app->appName == "Unknown")
            ? "a Remote Control app "
            : "the " + app->appName + " version " + app->appVersion + " ");

    temp = temp + ((app->deviceName == "Unknown" || app->osVersion == "Unknown")
            ? "on my device.\n"
            : "on my " + app->deviceName + " (" + app->osVersion + ").\n");

    temp = temp + "My " + Server::Instance()->getServerOs() + " pc has the Remote Control Server version "
            + Server::Instance()->getServerVersionName() + " installed.\n";

    return temp;
}

bool Helper::sendMail(QString address, QString subject, QString body)
{
    QString params = address;
    if (!params.toLower().startsWith("mailto:")) params = "mailto:" + params;
    if (subject.length() > 0) params = params + "?subject=" + subject;
    if (body.length() > 0)
    {
        params = params + ((subject.length() == 0) ? "?" : "&");
        params = params + "body=" + body;
    }
    return QDesktopServices::openUrl(QUrl(params));
}


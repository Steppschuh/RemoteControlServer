#ifndef SERVER_H
#define SERVER_H

#include <QList>
#include <QObject>
#include <QString>

#include "app.h"
#include "logger.h"
#include "mainwindow.h"

class Server
{
public:
    static Server *Instance();

    QString userName;
    QString status;

    void finish();
    QString getServerName();
    App *getApp(QString &ip);
    QString getServerOs();
    QString getServerVersionName();
    App *getCurrentApp();
    void startProcess(QString path);

private:
    Server();
    static Server *instance;

//    // advanced SettingsWindow
//    // upgrade UpgradeWindow
//    // upgradeHandle
//    // pointer
    QList<App*> *apps;

    bool isLatestServerRunning();
//    void showAdvancedWindow();
//    // void showAdvancedWindow(tab);
//    void showUpgradeWindow();
//    void showUpgradeWindowInvalidated();

};

#endif // SERVER_H

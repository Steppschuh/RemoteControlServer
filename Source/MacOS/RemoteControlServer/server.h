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

//    QMainWindow *gui;
    QString status;

    void finish();
    QString getServerName();
    App *getApp(QString &ip);

private:
    Server();
    static Server *instance;

//    // advanced SettingsWindow
//    // upgrade UpgradeWindow
//    // upgradeHandle
//    // pointer
    QList<App*> *apps;

    bool isLatestServerRunning();
    App *getCurrentApp();
//    void showAdvancedWindow();
//    // void showAdvancedWindow(tab);
//    void showUpgradeWindow();
//    void showUpgradeWindowInvalidated();
    QString getServerVersionName();
    QString getServerOs();

};

#endif // SERVER_H

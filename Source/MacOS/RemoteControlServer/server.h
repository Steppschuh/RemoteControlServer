#ifndef SERVER_H
#define SERVER_H

#include <QList>
#include <QString>

#include "app.h"
#include "logger.h"
#include "mainwindow.h"

namespace Ui { class MainWindow; }

class Server
{
public:
    static Server *Instance();

    void finish();
    QString getServerName();

private:
    Server();
    static Server *instance;

//    // advanced SettingsWindow
//    // upgrade UpgradeWindow
//    // upgradeHandle
//    // pointer
    QString status;
    QList<App> *apps;

    bool isLatestServerRunning();
    App getApp(QString ip);
    App getCurrentApp();
//    void showAdvancedWindow();
//    // void showAdvancedWindow(tab);
//    void showUpgradeWindow();
//    void showUpgradeWindowInvalidated();
    QString getServerVersionName();
    QString getServerOs();

};

#endif // SERVER_H

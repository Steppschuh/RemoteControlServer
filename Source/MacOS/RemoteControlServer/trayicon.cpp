#include "app.h"
#include "authentication.h"
#include "network.h"
#include "server.h"
#include "trayicon.h"

#include <QDebug>

TrayIcon::TrayIcon()
{
    serverIp = new QAction(this);
    serverVersion = new QAction(this);
    serverProtected = new QAction(this);
    serverStatus = new QAction(this);

    appIp = new QAction(this);
    appVersion = new QAction(this);
    appDevice = new QAction(this);
    appStatus = new QAction(this);

    openSettingsWindow = new QAction(this);
    openSettingsWindow->setText("Settings");

    quit = new QAction(this);
    quit->setText("Quit");

    this->setIcon(QIcon(":icons/SystemTrayIcon.png"));
    trayIconMenu = new QMenu();

    trayIconMenu->addAction("Server");
    trayIconMenu->actions().at(0)->setEnabled(false);
    trayIconMenu->addAction(serverIp);
    trayIconMenu->addAction(serverVersion);
    trayIconMenu->addAction(serverProtected);
    trayIconMenu->addAction(serverStatus);
    trayIconMenu->addSeparator();

    trayIconMenu->addAction("App");
    trayIconMenu->actions().at(6)->setEnabled(false);
    trayIconMenu->addAction(appIp);
    trayIconMenu->addAction(appVersion);
    trayIconMenu->addAction(appDevice);
    trayIconMenu->addAction(appStatus);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(openSettingsWindow);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quit);

    this->setContextMenu(trayIconMenu);
    this->show();

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onClick()));
}

void TrayIcon::onClick()
{
    serverIp->setText("IP Address: " + Network::Instance()->getServerIp());
    serverVersion->setText("Version: " + Server::Instance()->getServerVersionName());
    QString protectedText = "Protected: ";
    protectedText += (Authentication::Instance()->isProtected()) ? "Yes" : "No";
    serverProtected->setText(protectedText);
    serverStatus->setText("Status: " + Server::Instance()->status);

    App *currentApp = Server::Instance()->getCurrentApp();
    appIp->setText("IP Address: " + currentApp->ip);
    appVersion->setText("Version: " + currentApp->appVersion);
    appDevice->setText("Device: " + currentApp->deviceName);
    appStatus->setText("Status: " + currentApp->status);
}


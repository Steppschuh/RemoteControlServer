#ifndef TRAYICON_H
#define TRAYICON_H

//#include "mainwindow.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QObject>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    TrayIcon();

    QAction* openSettingsWindow;
    QAction* quit;

public slots:
    void onClick();

private:

    QMenu* trayIconMenu;

    QAction* serverIp;
    QAction* serverVersion;
    QAction* serverProtected;
    QAction* serverStatus;

    QAction* appIp;
    QAction* appVersion;
    QAction* appDevice;
    QAction* appStatus;

};

#endif // TRAYICON_H

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QObject>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    TrayIcon(QObject *parent);

    QAction* openSettingsWindow;
    QAction* quit;

public slots:
    void onClick();
    void showNotification(QString title, QString text);

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

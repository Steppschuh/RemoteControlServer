#ifndef SERVER_H
#define SERVER_H

#include <QList>
#include <QObject>
#include <QString>

#include "app.h"
#include "logger.h"
#include "mainwindow.h"
#include "pointerwindow.h"

class Server : public QObject
{
    Q_OBJECT

public:
    static Server *Instance();

    QString userName;
    QString status;
    PointerWindow *pointer;

    void initializeAsync();
    void finish();
    QString getServerName();
    App *getApp(QString ip);
    QString getServerOs();
    QString getServerVersionName();
    App *getCurrentApp();
    void startProcess(QString path);
    void showNotification(QString title, QString text);
    void showErrorDialog(QString title, QString text);

signals:
    void newErrorMessage(QString title, QString text);
    void newNotification(QString title, QString text);

private:
    Server();
    static Server *instance;

    QList<App*> *apps;

    bool isLatestServerRunning();
};

#endif // SERVER_H

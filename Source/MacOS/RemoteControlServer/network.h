#ifndef NETWORK_H
#define NETWORK_H

#include <QString>
#include <QStringList>

#include "command.h"

#include <QObject>

class Network : public QObject
{
    Q_OBJECT

public:
    static Network* Instance();

    QStringList *hostIps;
    long int commandCount;

    QString getServerIp();
    void sendCommand(Command &command);
    void loadInBrowser(QString &url);
    bool isValidIp(QString &ip);

signals:
    void sendData(Command *command);
    void sendDataRetry(Command *command);
    void sendDataUntilReceived(Command *command);

private:
    Network();
    static Network* instance;

    const QString localHost;

    bool checkListenersRunning();
    void getHostIps();
};

#endif // NETWORK_H

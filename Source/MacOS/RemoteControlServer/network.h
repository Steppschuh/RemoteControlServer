#ifndef NETWORK_H
#define NETWORK_H

#include <QString>
#include <QStringList>

#include "command.h"

class Network
{
public:
    static Network* Instance();

    QStringList *hostIps;
    long int commandCount;

    QString getServerIp();
    void sendCommand(Command &command);
    void loadInBrowser(QString &url);
    bool isValidIp(QString &ip);

private:
    Network();
    static Network* instance;

    const QString localHost;

    bool checkListenersRunning();
    void getHostIps();
};

#endif // NETWORK_H

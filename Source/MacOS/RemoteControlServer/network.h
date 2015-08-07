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

private:
    Network();
    static Network* instance;

    const QString localHost;

    bool checkListenersRunning();
    void getHostIps();
    bool isValidIp(QString &ip);
};

#endif // NETWORK_H

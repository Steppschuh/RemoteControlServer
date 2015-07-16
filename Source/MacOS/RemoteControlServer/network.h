#ifndef NETWORK_H
#define NETWORK_H

#include <QString>
#include <QStringList>

#include "command.h"
#include "tcp.h"
#include "udp.h"

class Network
{
public:
    static Network* Instance();

    QStringList *hostIps;

    TCP *tcp;
    UDP *udp;

    QString getServerIp();

private:
    Network();
    static Network* instance;

//    const QString localHost;
//    //anyIpEndPoint;
//    long int commandCount;

//    bool checkListenersRunning();
//    void getHostIps();
//    void sendCommand(Command *command);
//    void getWebRequestAsync(QString url, void (*callback)(QString, bool));
//    void getWebRequest(QString url, void (*callback)(QString, bool));
//    QString getWebRequest(QString url);
//    void loadInBrowser(QString url);
//    bool isValidIp(QString ip);
};

#endif // NETWORK_H

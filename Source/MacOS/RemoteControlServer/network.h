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

    TCP *tcp;
    UDP *udp;

private:
    Network();
    static Network* instance;

//    const QString localHost;
//    QStringList hostIps;
//    //anyIpEndPoint;
//    long int commandCount;

//    bool checkListenersRunning();
//    void getHostIps();
//    QString getServerIp();
//    void sendCommand(Command *command);
//    void getWebRequestAsync(QString url, void (*callback)(QString, bool));
//    void getWebRequest(QString url, void (*callback)(QString, bool));
//    QString getWebRequest(QString url);
//    void loadInBrowser(QString url);
//    bool isValidIp(QString ip);
};

#endif // NETWORK_H

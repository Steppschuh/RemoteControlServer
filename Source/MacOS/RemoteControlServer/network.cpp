#include "network.h"
#include "remote.h"

Network* Network::instance = NULL;

Network* Network::Instance()
{
    if (!instance)
    {
        instance = new Network();
    }
    return instance;
}

Network::Network()//:
//    localHost("127.0.0.1")
{
    hostIps = new QStringList();
}

QString Network::getServerIp()
{
    QString ip = "Unknown";
    if (hostIps->length() > 0)
    {
        ip = hostIps->at(hostIps->length() - 1);
    }
    if (hostIps->contains(Remote::Instance()->lastCommand->destination))
    {
        ip = Remote::Instance()->lastCommand->destination;
    }
    return ip;
}

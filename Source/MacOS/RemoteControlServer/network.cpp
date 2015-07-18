#include "logger.h"
#include "network.h"
#include "remote.h"
#include "tcp.h"
#include "udp.h"

#include <QNetworkInterface>
#include <QtConcurrent>

#include <QDebug>

Network* Network::instance = NULL;

Network* Network::Instance()
{
    if (!instance)
    {
        instance = new Network();
    }
    return instance;
}

Network::Network():
    localHost("127.0.0.1")
{
    hostIps = new QStringList();
    getHostIps();
    TCP::Instance();
    UDP::Instance();

    checkListenersRunning();
}

bool Network::checkListenersRunning()
{
    if (TCP::Instance()->isListening && UDP::Instance()->isListening)
    {
        return true;
    }
    else
    {
        Logger::Instance()->add("Network initialization failed, not all listeners could be started.");
        // Server.gui.showDialog...
        return false;
    }
}

void Network::getHostIps()
{
    Logger::Instance()->add("Active Wi-Fi network adapters:");
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        QString ip = address.toString();
        hostIps->append(ip);
        Logger::Instance()->add(" - " + ip);
    }
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

void Network::sendCommandThread(Command &command)
{
    switch (command.priority)
    {
    case Command::PRIORITY_LOW:
        UDP::Instance()->sendData(command);
        break;
    case Command::PRIORITY_MEDIUM:
        TCP::Instance()->sendData(command);
        break;
    case Command::PRIORITY_HIGH:
        TCP::Instance()->sendDataRetry(command);
        break;
    case Command::PRIORITY_INDISPENSABLE:
        TCP::Instance()->sendDataUntilReceived(command);
        break;
    default:
        TCP::Instance()->sendData(command);
    }
}

void Network::sendCommand(Command &command)
{
    sendCommandThread(command);
}

void Network::loadInBrowser(QString &url)
{
    //Server.gui.loadInBrowser
}

bool Network::isValidIp(QString &ip)
{
    QHostAddress ipAddress;
    return (ipAddress.setAddress(ip)) ? true : false;
}

#include "logger.h"
#include "network.h"
#include "remote.h"
#include "server.h"
#include "tcp.h"
#include "udp.h"

#include <QDesktopServices>
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
    Logger::Instance()->add("Initializing network");
    hostIps = new QStringList();
    getHostIps();
    TCP::Instance();
    UDP::Instance();

    commandCount = 0;

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
        Server::Instance()->showErrorDialog("Error", "Network initialization failed, not all listeners could be started.\n"
                                            + "Make sure that only one instance of the Remote Control Server is running.");
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
    for (int i = 0; i < hostIps->length(); ++i)
    {
        if (hostIps->at(i).startsWith("192"))
        {
            ip = hostIps->at(i);
            break;
        }
    }
    if (hostIps->contains(Remote::Instance()->lastCommand->destination))
    {
        ip = Remote::Instance()->lastCommand->destination;
    }
    return ip;
}

void Network::sendCommand(Command &command)
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

void Network::loadInBrowser(QString &url)
{
    QDesktopServices::openUrl(url);
}

bool Network::isValidIp(QString &ip)
{
    QHostAddress ipAddress;
    return (ipAddress.setAddress(ip)) ? true : false;
}

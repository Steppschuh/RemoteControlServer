#include "logger.h"
#include "network.h"
#include "remote.h"
#include "server.h"
#include "tcp.h"
#include "udp.h"

#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkRequest>
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

    QtConcurrent::run(this, &Network::checkListenersRunning);

    connect(this, SIGNAL(sendData(Command*)), TCP::Instance(), SLOT(sendData(Command*)));
    connect(this, SIGNAL(sendDataRetry(Command*)), TCP::Instance(), SLOT(sendDataRetry(Command*)));
    connect(this, SIGNAL(sendDataUntilReceived(Command*)), TCP::Instance(), SLOT(sendDataUntilReceived(Command*)));
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
        Server::Instance()->showErrorDialog("Error", QString("Network initialization failed, not all listeners could be started.\n")
                                            + QString("Make sure that only one instance of the Remote Control Server is running."));
        return false;
    }
}

void Network::getHostIps()
{
    Logger::Instance()->add("Active Wi-Fi network adapters:");
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost)
        {
            QString ip = address.toString();
            hostIps->append(ip);
            Logger::Instance()->add(" - " + ip);
        }
    }
}

QString Network::getServerIp()
{
    QString ip = "Unknown";
    for (int i = 0; i < hostIps->length(); ++i)
    {
        if (hostIps->at(i).startsWith("192.168") || hostIps->at(i).startsWith("10.") || hostIps->at(i).startsWith("172."))
        {
            ip = hostIps->at(i);
            break;
        }
    }
    if (Remote::Instance()->lastCommandIsInitialized && hostIps->contains(Remote::Instance()->lastCommand->destination))
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
//        UDP::Instance()->sendData(command);
        // send Date via UDP (Not implemented, nor used)
        break;
    case Command::PRIORITY_MEDIUM:
        emit sendData(&command);
        break;
    case Command::PRIORITY_HIGH:
        emit sendDataRetry(&command);
        break;
    case Command::PRIORITY_INDISPENSABLE:
        emit sendDataUntilReceived(&command);
        break;
    default:
        emit sendData(&command);
    }
}

void Network::loadInBrowser(QString &url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl(url)));
}

bool Network::isValidIp(QString &ip)
{
    QHostAddress ipAddress;
    return (ipAddress.setAddress(ip)) ? true : false;
}

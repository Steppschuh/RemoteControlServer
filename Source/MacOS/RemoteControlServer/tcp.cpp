#include "logger.h"
#include "tcp.h"

#include <QByteArray>

TCP* TCP::instance = NULL;

TCP* TCP::Instance()
{
    if (!instance)
    {
        instance = new TCP();
    }
    return instance;
}

TCP::TCP() :
    portReceive(1925),
//    portSend(1927),
    buffer(1024)
//    receiveTimeout(2000),
//    retries(3),
//    retryTimeout(1100),
//    sendTimeout(1000)
{
    isListening = false;

    Logger::Instance()->add("Initializing TCP");
    startListener();
}

bool TCP::sendData(Command *command)
{
    bool received = false;
}

void TCP::startListener()
{
    if (!isListening)
    {
        isListening = true;
        tcpServer = new QTcpServer();
        bool success = tcpServer->listen(QHostAddress::Any, portReceive);
        if (success)
        {
            connect(tcpServer, SIGNAL(newConnection()), this, SLOT(listenTimerTick()));
            Logger::Instance()->add("TCP listener started at port " + QString(portReceive));
        }
        else
        {
            Logger::Instance()->add("Error while starting TCP listener");
        }
    }
    else
    {
        Logger::Instance()->add("TCP listener already running at port " + QString(portReceive));
    }
}

void TCP::stopListener()
{

}

void TCP::listenTimerTick()
{
    listen();
}

void TCP::listen()
{
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    char messageData[buffer];
//    socket->read(messageData, buffer);
}


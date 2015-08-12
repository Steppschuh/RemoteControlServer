#include "logger.h"
#include "tcp.h"

#include <QByteArray>
#include <QTcpSocket>
#include <QThread>

#include <QDebug>

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
    portSend(1927),
    buffer(1024),
    receiveTimeout(2000),
    retries(3),
    retryTimeout(1100),
    sendTimeout(1000)
{
    isListening = false;

    Logger::Instance()->add("Initializing TCP");
    startListener();
}

bool TCP::sendData(Command &command)
{
    bool received = false;
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(command.destination, portSend);
    if (socket->waitForConnected(sendTimeout))
    {
        socket->write(*command.data);
        received = true;
        socket->close();
    }
    else
    {
        Logger::Instance()->add("Unable to send command to " + command.destination + ":" + QString::number(portSend));
    }
    return received;
}

void TCP::sendDataRetry(Command &command)
{
    for (int i = 0; i < retries; ++i)
    {
        if (sendData(command))
        {
            break;
        }
    }
}

void TCP::sendDataUntilReceived(Command &command)
{
    bool received = false;
    while (!received)
    {
        received = sendData(command);
        QThread::sleep(retryTimeout);
    }
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
            Logger::Instance()->add("TCP listener started at port " + QString::number(portReceive));
        }
        else
        {
            isListening = false;
            Logger::Instance()->add("Error while starting TCP listener");
        }
    }
    else
    {
        Logger::Instance()->add("TCP listener already running at port " + QString::number(portReceive));
    }
}

void TCP::stopListener()
{
    tcpServer->close();
}

void TCP::listenTimerTick()
{
    listen();
}

void TCP::listen()
{
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    if (socket->waitForReadyRead())
    {
        QByteArray messageData = socket->readAll();
        Command *command = new Command();
        QString ip = socket->peerAddress().toString();
        if (ip.startsWith("::ffff:")) ip = ip.right(ip.length() - 7);
        command->source = ip;
        ip = socket->localAddress().toString();
        if (ip.startsWith("::ffff:")) ip = ip.right(ip.length() - 7);
        command->destination = ip;
        command->data = &messageData;
        command->process();
    }
}


#include "logger.h"
#include "tcp.h"
#include "tcpthread.h"

#include <QByteArray>
#include <QTcpSocket>
#include <QThread>

#include <QDebug>
#include <QtConcurrent>

TCP* TCP::instance = NULL;

TCP* TCP::Instance()
{
    if (!instance)
    {
        instance = new TCP();
    }
    return instance;
}

TCP::TCP(QObject *parent) :
    QTcpServer(parent),
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

void TCP::sendData(Command *command)
{
    QtConcurrent::run(this, &TCP::sendDataThread, command);
}

bool TCP::sendDataThread(Command *command) {
    bool received = false;
    QTcpSocket *socket = new QTcpSocket(0);
    socket->connectToHost(command->destination, portSend);

    if (socket->waitForConnected(sendTimeout))
    {
        socket->write(*command->data);
        received = true;
        socket->close();
    }
    else
    {
        Logger::Instance()->add("Unable to send command to " + command->destination + ":" + QString::number(portSend));
    }
    return received;
}

void TCP::sendDataRetry(Command *command)
{
    QtConcurrent::run(this, &TCP::sendDataRetryThread, command);
}

void TCP::sendDataRetryThread(Command *command)
{
    for (int i = 0; i < retries; ++i)
    {
        if (sendDataThread(command))
        {
            break;
        }
    }
}

void TCP::sendDataUntilReceived(Command *command)
{
    QtConcurrent::run(this, &TCP::sendDataUntilReceivedThread, command);
}

void TCP::sendDataUntilReceivedThread(Command *command)
{
    bool received = false;
    while (!received)
    {
        received = sendDataThread(command);
        QThread::sleep(retryTimeout);
    }
}

void TCP::startListener()
{
    if (!isListening)
    {
        isListening = true;
//        tcpServer = new QTcpServer();
//        bool success = tcpServer->listen(QHostAddress::Any, portReceive);
        bool success = this->listen(QHostAddress::Any, portReceive);
        if (success)
        {
//            connect(tcpServer, SIGNAL(newConnection()), this, SLOT(listenTimerTick()));
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
    this->close();
}

void TCP::listenTimerTick()
{
//    listen();
}

void TCP::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
//    qDebug() << socketDescriptor << " Connecting...";

    TCPThread *thread = new TCPThread(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}


#include "logger.h"
#include "tcp.h"
#include "tcpthread.h"

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

bool TCP::sendData(Command &command)
{
    qint64 start = QDateTime::currentMSecsSinceEpoch();
    bool received = false;
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(command.destination, portSend);
    if (socket->waitForConnected(sendTimeout))
    {
        qDebug() << "Data sending: " << command.data->length();
        socket->write(*command.data);
//        qDebug() << "Data sent";
        received = true;
        socket->close();
        qDebug() << "Sending succeeded in ms:" << QDateTime::currentMSecsSinceEpoch() - start;
    }
    else
    {
        Logger::Instance()->add("Unable to send command to " + command.destination + ":" + QString::number(portSend));
        qDebug() << "Sending failed in ms:" << QDateTime::currentMSecsSinceEpoch() - start;
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

//void TCP::listen()
//{
//    QTime t;
//    t.start();
//    QTcpSocket *socket = tcpServer->nextPendingConnection();
//    if (socket->waitForReadyRead())
//    {
//        QByteArray messageData = socket->readAll();
//        Command *command = new Command();
//        QString ip = socket->peerAddress().toString();
//        if (ip.startsWith("::ffff:")) ip = ip.right(ip.length() - 7);
//        command->source = ip;
//        ip = socket->localAddress().toString();
//        if (ip.startsWith("::ffff:")) ip = ip.right(ip.length() - 7);
//        command->destination = ip;
//        command->data = &messageData;
//        qDebug() << "TCP listen in ms " << t.elapsed();
//        command->process();
//    }
//}


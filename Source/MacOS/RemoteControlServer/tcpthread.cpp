#include "tcpthread.h"
#include "command.h"

#include <QHostAddress>

#include <QDateTime>

TCPThread::TCPThread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void TCPThread::run()
{
    // thread starts here

    socket = new QTcpSocket();

    // set the ID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();
}

void TCPThread::readyRead()
{
    // get the information
    qDebug() << socketDescriptor << "Thread start" << QDateTime::currentMSecsSinceEpoch();
    QByteArray Data = socket->readAll();

    // will write on server side window

    Command *command = new Command();
    QString ip = socket->peerAddress().toString();
    if (ip.startsWith("::ffff:")) ip = ip.right(ip.length() - 7);
    command->source = ip;
    ip = socket->localAddress().toString();
    if (ip.startsWith("::ffff:")) ip = ip.right(ip.length() - 7);
    command->destination = ip;
    command->data = &Data;
    command->process();
    commandProcessed = true;
//    qDebug() << socketDescriptor << "command is Processed equals true";
    if (commandProcessed && isDisconnected)
    {
//        qDebug() << socketDescriptor << "exit after reading";
        exit(0);
    }
//    qDebug() << socketDescriptor << isDisconnected;
    qDebug() << socketDescriptor << "Thread end" << QDateTime::currentMSecsSinceEpoch();
}

void TCPThread::disconnected()
{
//    qDebug() << socketDescriptor << " Disconnected";

    socket->deleteLater();
    isDisconnected = true;
    if (commandProcessed && isDisconnected)
    {
//        qDebug() << socketDescriptor << "exit after disconnected";
        exit(0);
    }
}

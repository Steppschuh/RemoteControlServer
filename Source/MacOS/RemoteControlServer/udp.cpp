#include "logger.h"
#include "remote.h"
#include "udp.h"

#include <QByteArray>
#include <QDebug>

UDP* UDP::instance = NULL;

UDP* UDP::Instance()
{
    if (!instance)
    {
        instance = new UDP();
    }
    return instance;
}

UDP::UDP() :
    port(1926),
    timeout(2000)
{
    isListening = false;
    Logger::Instance()->add("Initializing UDP");

    startListener();
}

void UDP::sendData(Command &command)
{

}

void UDP::startListener()
{
    if (!isListening)
    {
        isListening = true;
        udpSocket = new QUdpSocket(this);
        bool success = udpSocket->bind(QHostAddress::Any, port);
        if (success)
        {
            connect(udpSocket, SIGNAL(readyRead()), this, SLOT(listen()));
            Logger::Instance()->add("UDP listener started listening at port " + QString(port));
        }
        else
        {
            Logger::Instance()->add("Error while starting UDP listener");
        }
    }
    else
    {
        Logger::Instance()->add("UDP listener already running at port " + QString(port));
    }
}

void UDP::stopListener()
{
    udpSocket->close();
}

void UDP::listen()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray *message_data = new QByteArray();
        message_data->resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(message_data->data(), message_data->size(), &sender, &senderPort);
        if (message_data->length() > 0)
        {
            Command *command = new Command();
            command->source = Remote::Instance()->lastCommand->source;
            command->destination = Remote::Instance()->lastCommand->destination;
            command->data = *message_data;
            command->process();
        }
        else
        {
            Logger::Instance()->add("Error while receiving UDP data");
        }
    }
}

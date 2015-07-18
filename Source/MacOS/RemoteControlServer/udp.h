#ifndef UDP_H
#define UDP_H

#include <command.h>

#include <QUdpSocket>

class UDP : public QObject
{
    Q_OBJECT

public:
    static UDP *Instance();

    bool isListening;

    void sendData(Command &command);

public slots:
    void listen();

private:
    UDP();
    static UDP *instance;

    const int port;
    const int timeout;

    QUdpSocket *udpSocket;

    void startListener();
    void stopListener();
};

#endif // UDP_H

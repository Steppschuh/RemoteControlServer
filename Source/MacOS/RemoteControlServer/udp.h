#ifndef UDP_H
#define UDP_H

#include <command.h>

#include <QThread>
#include <QUdpSocket>

class UDP : public QObject
{
    Q_OBJECT

public:
    static UDP *Instance();

    bool isListening;

public slots:
    void listen();
    void restartListener();

private:
    explicit UDP(QObject *parent = 0);
    static UDP *instance;


    const int port;
    const int timeout;

    QUdpSocket *udpSocket;

    void startListener(bool logMessages);
    void stopListener();

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // UDP_H

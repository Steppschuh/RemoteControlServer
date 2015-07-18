#ifndef TCP_H
#define TCP_H

#include <QTcpServer>

#include "command.h"

class TCP : public QObject
{
    Q_OBJECT

public:
    static TCP* Instance();

    bool isListening;

    bool sendData(Command &command);
    void sendDataRetry(Command &command);
    void sendDataUntilReceived(Command &command);
    void stopListener();

public slots:
    void listenTimerTick();

private:
    TCP();
    static TCP* instance;

    const int portReceive;
    const int portSend;

    const int buffer;
    const int receiveTimeout;

    const int retries;
    const int retryTimeout;
    const int sendTimeout;

    QTcpServer *tcpServer;

    void startListener();
    void listen();
};

#endif // TCP_H

#ifndef TCP_H
#define TCP_H

#include <QTcpServer>
#include <QThread>

#include "command.h"

class TCP : public QObject
{
    Q_OBJECT

public:
    static TCP* Instance();

    bool isListening;

    void stopListener();

public slots:
    void listenTimerTick();
    void sendData(Command *command);
    void sendDataRetry(Command *command);
    void sendDataUntilReceived(Command *command);

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

    QThread *tcpThread;

    void startListener();
    void listen();

    bool sendDataThread(Command *command);
    void sendDataRetryThread(Command *command);
    void sendDataUntilReceivedThread(Command *command);
};

#endif // TCP_H

#ifndef TCP_H
#define TCP_H

#include <logger.h>
#include <QTimer>

class TCP : public QObject
{
    Q_OBJECT

public:
    static TCP* Instance();

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

    bool isListening;
    QTimer *listenTimer;
};

#endif // TCP_H

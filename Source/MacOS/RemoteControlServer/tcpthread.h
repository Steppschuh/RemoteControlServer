#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class TCPThread : public QThread
{
    Q_OBJECT
public:
    explicit TCPThread(qintptr ID, QObject *parent = 0);

    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;

    bool commandProcessed = false;
    bool isDisconnected = false;
};

#endif // TCPTHREAD_H

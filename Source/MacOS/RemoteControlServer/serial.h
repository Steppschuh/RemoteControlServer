#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>
#include <QString>

#include "command.h"

class Serial
{
public:
    static Serial *Instance();

    void closeSerialPort();
    bool openSerialPort(QString name);
    void sendMessage(QString message);
    void sendCommand(Command &command);

private:
    Serial();
    static Serial *instance;

    QString currentSerialPortName;
//    const int writeTimeout;
    QSerialPort *serialPort;
    bool isSending;
    bool shouldListen;

    void stopReading();
};

#endif // SERIAL_H

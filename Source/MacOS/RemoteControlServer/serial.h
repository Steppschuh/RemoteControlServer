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

    bool openSerialPort(QString name);
    void stopReading();
};

#endif // SERIAL_H

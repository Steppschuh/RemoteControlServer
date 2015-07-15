#ifndef SERIAL_H
#define SERIAL_H

#include <QString>

#include "command.h"

class Serial
{
public:
    static Serial *Instance();

    void closeSerialPort();

private:
    Serial();
    static Serial *instance;

//    QString currentSerialPortName;
//    const char commandStart;
//    const char commandEnd;
//    const int writeTimeout;
//    //serialPort
//    bool isSending;
//    bool shouldListen;

//    bool openSerialPort(QString name);
//    void sendMessage(QString message);
//    void sendMessageAsync(QString message);
//    void startReading();
//    void stopReading();
//    void read();
//    // SerialPort_DataReceived
//    void sendCommand(Command *command);
};

#endif // SERIAL_H

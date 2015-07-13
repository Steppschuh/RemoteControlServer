#ifndef SERIAL_H
#define SERIAL_H

#include <QString>

#include "command.h"

class Serial
{
public:
    Serial *Instance();

private:
    Serial();
    Serial *instance;

//    QString currentSerialPortName;
//    const char commandStart;
//    const char commandEnd;
//    const int writeTimeout;
//    //serialPort
//    bool isSending;
//    bool shouldListen;

//    bool openSerialPort(QString name);
//    void closeSerialPort();
//    void sendMessage(QString message);
//    void sendMessageAsync(QString message);
//    void startReading();
//    void stopReading();
//    void read();
//    // SerialPort_DataReceived
//    void sendCommand(Command *command);
};

#endif // SERIAL_H

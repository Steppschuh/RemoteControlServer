#include "converter.h"
#include "logger.h"
#include "serial.h"
#include "settings.h"

#include <QException>
#include <QSerialPortInfo>

Serial *Serial::instance = NULL;

Serial* Serial::Instance()
{
    if (!instance)
    {
        instance = new Serial();
    }
    return instance;
}

Serial::Serial()
{
    serialPort = NULL;
}

bool Serial::openSerialPort(QString name)
{
    if (name.toLower() == "auto")
    {
        if (QSerialPortInfo::availablePorts().length() > 0) name = QSerialPortInfo::availablePorts().last().portName();
        else
        {
            Logger::Instance()->add("Unable to open serial port " + currentSerialPortName);
            return false;
        }
    }

    currentSerialPortName = name;

    serialPort = new QSerialPort();
    serialPort->setPortName(currentSerialPortName);
    serialPort->setRequestToSend(true);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setBaudRate(9600);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->open(QIODevice::ReadWrite);

    return true;
}

void Serial::closeSerialPort()
{
    stopReading();
    if (serialPort) serialPort->close();
}

void Serial::sendMessage(QString message)
{
    isSending = true;
    bool success = false;
    if (serialPort) success = true;
    if (!serialPort) success = openSerialPort(Settings::Instance()->serialPortName);
    if (!serialPort->isOpen()) success = openSerialPort(Settings::Instance()->serialPortName);

    if (success)
    {
        serialPort->write(message.toLatin1());
        Logger::Instance()->add("Sent to " + currentSerialPortName + ": " + message);
    }
    else
    {
        Logger::Instance()->add("Unable to send message to " + currentSerialPortName);
    }
    isSending = false;
}

void Serial::stopReading()
{
    shouldListen = false;
}

void Serial::sendCommand(Command &command)
{
    isSending = true;
    bool success = false;
    if (serialPort) success = true;
    if (!serialPort) success = openSerialPort(Settings::Instance()->serialPortName);
    if (!serialPort->isOpen()) success = openSerialPort(Settings::Instance()->serialPortName);

    if (success)
    {
        QByteArray *data = new QByteArray();
        data->append(60);
        data->append(*command.data);
        data->append(62);

        serialPort->write(*data);
        Logger::Instance()->add("Sent to " + currentSerialPortName + ": " + Converter::Instance()->commandToString(command));
    }
    else
    {
        Logger::Instance()->add("Unable to send message to " + currentSerialPortName);
    }
    isSending = false;
}

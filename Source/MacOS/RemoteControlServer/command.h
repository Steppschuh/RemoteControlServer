#ifndef COMMAND_H
#define COMMAND_H

#include <QByteArray>
#include <QString>

class Command
{
public:
    Command();

    enum
    {
        PRIORITY_LOW = 0, //UDP
        PRIORITY_MEDIUM = 1, //TCP
        PRIORITY_HIGH = 2, //TCP retry
        PRIORITY_INDISPENSABLE = 3, //TCP retry forever
    };

    QString source;
    QString destination;
    char priority;
    QByteArray data;
    char api;

    void send();
    void process();

private:
    char type;

    void parse();
    QString dataAsString();
    void log();
    bool isBroadcast();
    bool isConnectionCommand();
};

#endif // COMMAND_H

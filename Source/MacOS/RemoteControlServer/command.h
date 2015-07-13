#ifndef COMMAND_H
#define COMMAND_H

#include <QByteArray>
#include <QString>

class Command
{
public:
    Command();
    static const char PRIORITY_LOW; //UDP
    static const char PRIORITY_MEDIUM; //TCP
    static const char PRIORITY_HIGH; //TCP retry
    static const char PRIORITY_INDISPENSABLE; //TCP retry forever

private:
    QString source;
    QString destination;
    char priority;
    char type;
    QByteArray data;
    char api;

//    void send();
//    void process();
//    void parse();
//    QString dataAsString();
//    void log();
//    bool isBroadcast();
//    bool isConnectionCommand();
};

#endif // COMMAND_H

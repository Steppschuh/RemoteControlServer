#ifndef NETWORK_H
#define NETWORK_H

#include <QString>
#include <QStringList>

class Network
{
public:
    static Network* Instance();

private:
    Network();
    static Network* instance;

    const QString localHost;
    QStringList hostIps;
};

#endif // NETWORK_H

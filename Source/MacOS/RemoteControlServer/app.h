#ifndef APP_H
#define APP_H

#include <QString>

#include "command.h"

class App
{
public:
    App();

    QString status;
    QString pin;
    QString appName;
    QString appVersion;

    QString deviceName;
    QString osVersion;

    QString lastControl;

    bool isConnected;

    void detectOs();
    QString getIp();
    void onBroadCast(Command &command);
    void onConnect();
    void onDisconnect();
    void onPause();
    void onResume();
    void setIp(QString newIp);


private:
    void answerBroadCast(Command &command);
    void requestPin(Command &command);

    QString ip;
};

#endif // APP_H

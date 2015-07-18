#ifndef APP_H
#define APP_H

#include <QString>

#include "command.h"

class App
{
public:
    App();

    QString ip;
    QString status;
    QString pin;

    bool isConnected;

    void setStatus(QString &newStatus);
    void setIsConnected(bool newValue);

private:
    QString appVersion;
    QString appName;
//    QString osVersion;
    QString deviceName;

//    char detectedOs;

//    QString lastControl;
//    QString lastCommand;

    void onConnect();
    void onDisconnect();
    void onPause();
    void onResume();
    void onBroadCast(Command &command);
    void answerBroadCast(Command &command);
//    void refuseBroadCast(Command *command);
//    void requestPin(Command *command);
//    void detectOs();
};

#endif // APP_H

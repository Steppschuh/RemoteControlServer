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
    QString appName;
    QString appVersion;

    QString deviceName;
    QString osVersion;

    QString lastControl;

    bool isConnected;

    void onConnect();
    void onDisconnect();
    void onPause();
    void onResume();
    void onBroadCast(Command &command);

    void detectOs();

private:
//    char detectedOs;

//    QString lastControl;
//    QString lastCommand;

    void answerBroadCast(Command &command);
    void refuseBroadCast(Command &command);
    void requestPin(Command &command);
};

#endif // APP_H

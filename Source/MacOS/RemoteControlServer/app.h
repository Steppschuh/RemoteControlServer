#ifndef APP_H
#define APP_H

#include <QString>

#include "command.h"

class App
{
public:
    App();

    QString ip;
    QString pin;

private:
    QString appVersion;
    QString appName;
//    QString osVersion;
    QString deviceName;
//    QString status;

//    char detectedOs;

//    QString lastControl;
//    QString lastCommand;

    bool isConnected;

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

#include "apiv2.h"
#include "apiv3.h"
#include "app.h"
#include "authentication.h"
#include "logger.h"
#include "settings.h"

App::App()
{
    ip = "Unknown";
    appVersion = "Unknown";
    appName = "Unknown";
    deviceName = "Unknown";
    status = "Unknown";
    pin = "";

    isConnected = false;
}

void App::setStatus(QString &newStatus)
{
    status = newStatus;
}

void App::setIsConnected(bool newValue)
{
    isConnected = newValue;
}

void App::onConnect()
{
    isConnected = true;
    Logger::Instance()->add(deviceName + " connected");
    // Server.gui.showNotification
    QString label = (appName == "Unknown" || appVersion == "Unknown")
            ? deviceName : appName + " " + appVersion + " on a " + deviceName;
    Logger::Instance()->trackEvent("App", "Connect", label);
    Settings::Instance()->showGuide = false;
}

void App::onDisconnect()
{
    isConnected = false;
    Logger::Instance()->add(deviceName + " disconnected");
}

void App::onPause()
{
    Logger::Instance()->add(deviceName + " paused the connection");
}

void App::onResume()
{
    Logger::Instance()->add(deviceName + " resumed the connection");
}

void App::onBroadCast(Command &command)
{
    Logger::Instance()->add("Connection request from " + ip);
    if (Authentication::Instance()->isAuthenticated(ip, pin))
    {
        Logger::Instance()->add("Allowing to connec");
    }
}

void App::answerBroadCast(Command &command)
{
    switch (command.api) {
    case (3):
//        ApiV2::Instance()->answerBroadcast
        break;
    }
}

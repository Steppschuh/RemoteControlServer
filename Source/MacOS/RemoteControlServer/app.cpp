#include "apiv2.h"
#include "apiv3.h"
#include "app.h"
#include "authentication.h"
#include "logger.h"
#include "server.h"
#include "settings.h"

#include <QDebug>

App::App()
{
    ip = "Unknown";
    appVersion = "Unknown";
    appName = "Unknown";
    deviceName = "Unknown";
    osVersion = "Unknown";
    deviceName = "Unknown";
    status = "Unknown";
    pin = "";

    lastControl = "Unknown";

    isConnected = false;
}

void App::onConnect()
{
    isConnected = true;
    Logger::Instance()->add(deviceName + " connected");
    Server::Instance()->showNotification("App connected", deviceName + " has connected to the Remote Control Server");
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
        answerBroadCast(command);
    }
    else if (Settings::Instance()->usePin)
    {
        Logger::Instance()->add("Requesting Pin");
        requestPin(command);
    }
    else
    {
        Logger::Instance()->add("Connection blocked");
        refuseBroadCast(command);
        Server::Instance()->showNotification("Connection blocked", "A connection attempt from " + ip + " has been blocked.");
    }
}

void App::answerBroadCast(Command &command)
{
    switch (command.api) {
    case 3:
        ApiV3::Instance()->answerBroadCast(*this);
        break;
    case 2:
    case 1:
        ApiV2::Instance()->answerBroadCast(*this);
        break;
    default:
        ApiV3::Instance()->answerBroadCast(*this);
        break;
    }
}

void App::refuseBroadCast(Command &command)
{
    // refuseBroadcast, do nothing
}

void App::requestPin(Command &command)
{
    switch (command.api) {
    case 3:
        ApiV3::Instance()->requestPin(*this);
        break;
    case 2:
    case 1:
        ApiV2::Instance()->requestPin(*this);
        break;
    default:
        ApiV3::Instance()->requestPin(*this);
    }
}

void App::setIp(QString newIp)
{
    if (newIp.startsWith("::ffff:")) ip =  newIp.right(newIp.length() - 7); // remove the first seven chars when ip is formatted as ipv6
    else ip = newIp;
}

QString App::getIp()
{
    return ip;
}

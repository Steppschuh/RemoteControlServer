#include "apiv1.h"
#include "apiv2.h"
#include "apiv3.h"
#include "authentication.h"
#include "command.h"
#include "logger.h"
#include "network.h"
#include "remote.h"
#include "server.h"

#include <QDebug>

Command::Command()
{
    source = "Unknown";
    destination = "Unknown";
    priority = 0;
    api = Remote::Instance()->latestApi;
    data = new QByteArray();
}

void Command::send()
{
    Network::Instance()->sendCommand(*this);
}

void Command::process()
{
    if (Authentication::Instance()->isAuthenticated(source, Server::Instance()->getApp(source)->pin) || isBroadcast() || isConnectionCommand())
    {
        parse();
    }
    else
    {
        Logger::Instance()->add("Refused a command from " + source);
        Logger::Instance()->add("Server protection is active");
    }
}

void Command::parse()
{
    if (data->length() > 0)
    {
        char commandByte = data->at(0);
        if (commandByte == ApiV3::Instance()->COMMAND_IDENTIFIER + 1)
        {
            api = Remote::Instance()->latestApi;
        }
        else if (commandByte == ApiV3::Instance()->COMMAND_IDENTIFIER)
        {
            api = 3;
        }
        else if (commandByte == ApiV2::Instance()->COMMAND_IDENTIFIER)
        {
            api = 2;
        }
        else
        {
            api = 1;
        }
        Network::Instance()->commandCount += 1;
        Remote::Instance()->processCommand(*this);
    }
    else
    {
        Logger::Instance()->add("Could not parse command");
    }
}

QString Command::dataAsString()
{
    return *data;
}

void Command::log()
{
    Logger::Instance()->add(source + ": " + dataAsString());
}

bool Command::isBroadcast()
{
    return (ApiV1::Instance()->isBroadcast(*this) || ApiV2::Instance()->isBroadcast(*this) || ApiV3::Instance()->isBroadcast(*this)) ? true : false;
}

bool Command::isConnectionCommand()
{
    return (ApiV3::Instance()->isConnectionCommand(*this)) ? true : false;
}

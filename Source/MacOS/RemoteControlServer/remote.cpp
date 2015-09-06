#include "apiv1.h"
#include "apiv2.h"
#include "apiv3.h"
#include "remote.h"
#include "server.h"

#include <QDebug>

Remote* Remote::instance = NULL;

Remote* Remote::Instance()
{
    if (!instance)
    {
        instance = new Remote();
    }
    return instance;
}

Remote::Remote()
{
    latestApi = 3;
    lastCommandIsInitialized = false;
}

void Remote::initializeLastCommand()
{
    lastCommand = new Command();
    lastCommandIsInitialized = true;
}

void Remote::processCommand(Command &command)
{
    lastCommand = &command;
    switch (command.api)
    {
    case 3:
        ApiV3::Instance()->parseCommand(command);
        break;
    case 2:
        ApiV2::Instance()->parseCommand(command);
        break;
    case 1:
        ApiV1::Instance()->parseCommand(command);
        break;
    default:
        ApiV3::Instance()->parseCommand(command);
        break;
    }
    Server::Instance()->status = "active";
    Server::Instance()->getApp(command.source)->status = "Active";
    Server::Instance()->getApp(command.source)->isConnected = false;
}

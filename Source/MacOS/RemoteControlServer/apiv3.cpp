#include "apiv3.h"
#include "app.h"
#include "converter.h"
#include "logger.h"
#include "network.h"
#include "server.h"

ApiV3* ApiV3::instance = NULL;

ApiV3* ApiV3::Instance()
{
    if (!instance)
    {
        instance = new ApiV3();
    }
    return instance;
}

ApiV3::ApiV3() :
    COMMAND_IDENTIFIER(127)
{

}

bool ApiV3::isBroadcast(Command &command)
{
    return false;
}

bool ApiV3::isConnectionCommand(Command &command)
{
    return false;
}

void ApiV3::parseCommand(Command &command)
{
    parseCommandThread(command);
}

void ApiV3::parseCommandThread(Command &command)
{
    if (command.data->length() >= 2)
    {
        switch (command.data->at(1))
        {
        case cmd_connection:
            parseConnectCommand(command);
            break;
        default:
            Logger::Instance()->add("Unknown command");
        }
    }
}

void ApiV3::parseConnectCommand(Command &command)
{
    App *app = Server::Instance()->getApp(command.source);
    if (command.data->length() >= 3)
    {
        switch (command.data->at(2))
        {
        case cmd_connection_reachable:
            app->ip = Converter::Instance()->byteToString(*command.data, 3);
            app->onBroadCast(command);
            break;
        }
    }
}

void ApiV3::answerBroadCast(App &app)
{
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = app.ip;
    command->priority = Command::PRIORITY_HIGH;
    QByteArray *serverName = new QByteArray();
    serverName->append(COMMAND_IDENTIFIER);
    serverName->append(cmd_get);
    serverName->append(cmd_get_server_name);
    serverName->append(Server::Instance()->getServerName().toUtf8());
    command->data = serverName;
    command->send();
}

#include "apiv2.h"
#include "converter.h"
#include "network.h"
#include "server.h"

#include <QDebug>

ApiV2* ApiV2::instance = NULL;

ApiV2* ApiV2::Instance()
{
    if (!instance)
    {
        instance = new ApiV2();
    }
    return instance;
}

ApiV2::ApiV2():
    COMMAND_IDENTIFIER(128)
{
}

bool ApiV2::isBroadcast(Command &command)
{
    QByteArray *byteArray = new QByteArray();
    byteArray->append(COMMAND_IDENTIFIER);
    byteArray->append(cmd_broadcast);
    return (command.data == byteArray) ? true : false;
}

void ApiV2::requestPin(App &app)
{
    QByteArray *data = new QByteArray();
    data->append(COMMAND_IDENTIFIER);
    data->append(cmd_request);
    data->append(cmd_request_pin);
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = app.ip;
    command->priority = Command::PRIORITY_HIGH;
    command->data = data;
    command->send();
}

void ApiV2::answerBroadCast(App &app)
{
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = app.ip;
    command->priority = Command::PRIORITY_HIGH;
    command->data = new QByteArray(Server::Instance()->getServerName().toUtf8());
    command->send();
}

void ApiV2::parseCommand(Command &command)
{
    char typeBit = command.data->at(1);
    if (typeBit < 20)
    {
        parseGeneralCommand(command);
    }
    else
    {
        parseRemoteCommand(command);
    }
}

void ApiV2::parseGeneralCommand(Command &command)
{
    App *app = Server::Instance()->getApp(command.source);

    switch (command.data->at(1))
    {
    case cmd_connect:
        app->onConnect();
        break;
    case cmd_disconnect:
        app->onDisconnect();
        break;
    case cmd_pause:
        app->onPause();
        break;
    case cmd_resume:
        app->onResume();
        break;
    case cmd_pin:
        app->pin = Converter::Instance()->byteToString(*command.data, 2);
        break;
    }
}

void ApiV2::parseRemoteCommand(Command &command)
{
//    switch (command.data.at(1)) {
//    case cmd_mouse:

//        break;
//    default:
//        break;
//    }
}

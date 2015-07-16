#include "apiv2.h"
#include "converter.h"
#include "network.h"
#include "server.h"

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
    COMMAND_IDENTIFIER(128),
    cmd_broadcast(16),
    cmd_request(27),
    cmd_request_pin(4)
{
}

bool ApiV2::isBroadcast(Command *command)
{
    QByteArray *byteArray = new QByteArray();
    byteArray->append(COMMAND_IDENTIFIER);
    byteArray->append(cmd_broadcast);
    return (command->data == byteArray) ? true : false;
}

void ApiV2::requestPin(App *app)
{
    QByteArray *data = new QByteArray();
    data->append(COMMAND_IDENTIFIER);
    data->append(cmd_request);
    data->append(cmd_request_pin);
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = app->ip;
    command->priority = Command::PRIORITY_HIGH;
//    command->data = data;
    command->send();
}

void ApiV2::answerBroadCast(App *app)
{
//    Command *command = new Command();
//    command->source = Network::Instance()->getServerIp();
//    command->destination = app->ip;
//    command->priority = Command::PRIORITY_HIGH;
//    command->data = Server::Instance()->getServerName().toUtf8();
//    command->send();
}

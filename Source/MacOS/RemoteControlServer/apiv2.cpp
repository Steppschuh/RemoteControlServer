#include "apiv2.h"
#include "converter.h"
#include "media.h"
#include "mousev2.h"
#include "network.h"
#include "screenshot.h"
#include "serial.h"
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
    command->destination = app.getIp();
    command->priority = Command::PRIORITY_HIGH;
    command->data = data;
    command->send();
}

void ApiV2::answerBroadCast(App &app)
{
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = app.getIp();
    command->priority = Command::PRIORITY_HIGH;
    command->data = new QByteArray(Server::Instance()->getServerName().toUtf8());
    command->send();
}

void ApiV2::parseCommand(Command &command)
{
    if (command.data->length() >= 2)
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
}

void ApiV2::parseGeneralCommand(Command &command)
{
    App *app = Server::Instance()->getApp(command.source);

    if (command.data->length() >= 2)
    {
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
        case cmd_control:
            if (command.data->length() >= 3)
            {
                switch (command.data->at(2))
                {
                case cmd_media:
                    app->lastControl = "Media";
                    break;
                case cmd_mouse:
                    app->lastControl = "Mouse";
                    break;
                case cmd_pointer:
                    app->lastControl = "Pointer";
                    break;
                case cmd_slideshow:
                    app->lastControl = "Slideshow";
                    break;
                case cmd_motion:
                    app->lastControl = "Motion";
                    break;
                case cmd_keyboard:
                    app->lastControl = "Keyboard";
                    break;
                case cmd_scroll:
                    app->lastControl = "Scroll";
                    break;
                case cmd_request:
                    app->lastControl = "Request";
                    break;
                case cmd_laser:
                    app->lastControl = "Laser";
                    break;
                case cmd_custom:
                    app->lastControl = "Custom";
                    break;
                default:
                    app->lastControl = "Unknown";
                    break;
                }
                //Logger::Instance()->add("Control set to " + app->lastControl);
            }
            break;
        default:
            //Logger::Instance()->add("Unkown general command");
            break;
        }
    }
}

void ApiV2::parseRemoteCommand(Command &command)
{
    if (command.data->length() >= 2)
    {
        switch (command.data->at(1)) {
        case cmd_mouse:
            parseMouseCommand(command);
            break;
        case cmd_pointer:
            parsePointerCommand(command);
            break;
        case cmd_media:
            parseMediaCommand(command);
            break;
        case cmd_request:
            parseRequest(command);
            break;
        case cmd_laser:
            parseLaserCommand(command);
            break;
        case cmd_custom:
            parseCustomCommand(command);
            break;
        default:
            //Logger::Instance()->add("Unkown remote command");
            break;
        }
    }
}

void ApiV2::parseMouseCommand(Command &command)
{
    if (command.data->length() >= 3)
    {
        switch (command.data->at(2))
        {
        case cmd_mouse_move:
            readableCommand = "Move cursor";
            MouseV2::Instance()->parseCursorMove(*command.data);
            break;
        case cmd_mouse_set:
            readableCommand = "Set cursor";
            MouseV2::Instance()->parseCursorSet(*command.data);
            break;
        case cmd_mouse_scroll:
            readableCommand = "Scroll";
            MouseV2::Instance()->parseScroll(*command.data);
            break;
        default:
            readableCommand = "Click";
            MouseV2::Instance()->parseClick(*command.data);
            break;
        }
    }
}

void ApiV2::parsePointerCommand(Command &command)
{
    if (command.data->length() >= 3)
    {
        switch (command.data->at(2))
        {
        case cmd_pointer_move:
            readableCommand = "Move";
            MouseV2::Instance()->parsePointer(*command.data);
            break;
        case cmd_pointer_start:
            readableCommand = "Start";
            Server::Instance()->pointer->showPointer();
            break;
        case cmd_pointer_end:
            readableCommand = "Hide";
            Server::Instance()->pointer->hidePointer();
            break;
        case cmd_pointer_calibrate:
            readableCommand = "Calibrate";
            MouseV2::Instance()->calibratePointer(*command.data);
            break;
        default:
            return;
        }
        //Logger::Instance()->add("Pointer: " + readableCommand);
    }
}

void ApiV2::parseLaserCommand(Command &command)
{
    readableCommand = "Move";
    MouseV2::Instance()->parseLaser(*command.data);
    //Logger::Instance()->add("Laser: " + readableCommand);
}

void ApiV2::parseMediaCommand(Command &command)
{
    if (command.data->length() >= 3)
    {
        switch (command.data->at(2))
        {
        case cmd_media_play:
            readableCommand = "Play";
            Media::Instance()->playMedia();
            break;
        case cmd_media_stop:
            readableCommand = "Stop";
            Media::Instance()->stopMedia();
            break;
        case cmd_media_prev:
            readableCommand = "Previous";
            Media::Instance()->previousMedia();
            break;
        case cmd_media_next:
            readableCommand = "Next";
            Media::Instance()->nextMedia();
            break;
        case cmd_media_voldown:
            readableCommand = "Volume down";
            Media::Instance()->volumeDown();
            break;
        case cmd_media_volup:
            readableCommand = "Volume up";
            Media::Instance()->volumeUp();
            break;
        case cmd_media_volmute:
            readableCommand = "Volume mute";
            Media::Instance()->volumeMute();
            break;
        case cmd_media_launch:
            readableCommand = "Launch player";
            Media::Instance()->launchPlayer();
            break;
        default:
            readableCommand = "Unkown";
            break;
        }
        //Logger::Instance()->add("Media: " + readableCommand);
    }
}

void ApiV2::parseRequest(Command &command)
{
    if (command.data->length() >= 3)
    {
        switch (command.data->at(2))
        {
        case cmd_request_screen:
            readableCommand = "Screenshot (normal)";
            Screenshot::Instance()->sendScreenshot(false);
            break;
        case cmd_request_screen_next:
            readableCommand = "Toggle screen";
            Screenshot::Instance()->toggleScreen();
            break;
        case cmd_request_screen_full:
            readableCommand = "Screenshot (full)";
            Screenshot::Instance()->sendScreenshot(true);
            break;
        }
        //Logger::Instance()->add("Request: " + readableCommand);
    }
}

void ApiV2::parseCustomCommand(Command &command)
{
    if (command.data->length() >= 3)
    {
        switch (command.data->at(2))
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 0:
            readableCommand = "Button " + QString(command.data->at(2));
            Serial::Instance()->sendMessage("<0" + QString(command.data->at(2)) + ">");
            break;
        case 10:
            readableCommand = "Volume up";
            Serial::Instance()->sendMessage("<10>");
            break;
        case 11:
            readableCommand = "Volume down";
            Serial::Instance()->sendMessage("<11>");
            break;
        case 12:
            readableCommand = "Scrollbar move";
            if (MouseV2::Instance()->X_Def == 0)
            {
                MouseV2::Instance()->X_Def = command.data->at(3);
            }
            else
            {
                MouseV2::Instance()->X_Rel = MouseV2::Instance()->X_Def - command.data->at(3);
                if (MouseV2::Instance()->X_Rel < MouseV2::Instance()->X_New - 10
                        || MouseV2::Instance()->X_Rel > MouseV2::Instance()->X_New + 10)
                {
                    int value = 64 + MouseV2::Instance()->X_Rel;
                    value = (value > 127) ? 127 : ((value < 0) ? 0 : value);
                    readableCommand = "Scrollbar " + QString::number(value);
                    Serial::Instance()->sendMessage("<12" + QString(char (value)) + ">");
                }
            }
            break;
        case 13:
            readableCommand = "Scrollbar down";
            MouseV2::Instance()->X_Def = command.data->at(3);
            Serial::Instance()->sendMessage("<13>");
            break;
        case 14:
            readableCommand = "Scrollbar up";
            MouseV2::Instance()->X_Def = 0;
            Serial::Instance()->sendMessage("<14>");
            break;
        default:
            readableCommand = "Unkown";
            break;
        }
        //Logger::Instance()->add("Custom: " + readableCommand);
    }
}

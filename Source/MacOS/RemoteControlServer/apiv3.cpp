#include "apiv3.h"
#include "app.h"
#include "authentication.h"
#include "converter.h"
#include "logger.h"
#include "mousev3.h"
#include "network.h"
#include "screenshot.h"
#include "serial.h"
#include "server.h"
#include "settings.h"

#ifdef Q_OS_MAC
    #include "keyboardmac.h"
    #include "mousemac.h"
#endif

#include <QByteArray>
#include <QProcess>
#include <QtConcurrent>

#include <QDebug>

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
    return ((command.data == QByteArray({COMMAND_IDENTIFIER, cmd_broadcast}))
             || (command.data->length() >= 3 && command.data->at(2) == cmd_connection_reachable))
            ? true : false;
}

bool ApiV3::isConnectionCommand(Command &command)
{
    return (command.data->length() >= 2 && (command.data->at(1) == cmd_connection || command.data->at(1) == cmd_set))
            ? true : false;
}

void ApiV3::requestPin(App &app)
{
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = app.ip;
    command->priority = Command::PRIORITY_HIGH;
    QByteArray *serverName = new QByteArray();
    serverName->append(COMMAND_IDENTIFIER).append(cmd_connection).append(cmd_connection_protected);
    serverName->append(Server::Instance()->userName.toUtf8());
    command->data = serverName;
    command->send();
}

void ApiV3::validatePin(App &app)
{
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = app.ip;
    command->priority = Command::PRIORITY_HIGH;
    QByteArray *commandData = new QByteArray();
    commandData->append(COMMAND_IDENTIFIER).append(cmd_connection);
    if(Authentication::Instance()->isAuthenticated(app.ip, app.pin))
    {
        commandData->append(cmd_connection_connect);
    }
    else
    {
        commandData->append(cmd_connection_disconnect);
    }
    command->data = commandData;
    command->send();
}

void ApiV3::answerBroadCast(App &app)
{
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = app.ip;
    command->priority = Command::PRIORITY_HIGH;
    QByteArray *serverName = new QByteArray();
    serverName->append(COMMAND_IDENTIFIER).append(cmd_get).append(cmd_get_server_name);
    serverName->append(Server::Instance()->userName.toUtf8());
    command->data = serverName;
    command->send();
}

void ApiV3::parseCommand(Command &command)
{
    if (command.data->length() >= 2)
    {
        switch (command.data->at(1))
        {
        case cmd_connection:
            parseConnectCommand(command);
            break;
        case cmd_get:
            answerGetRequest(command);
            break;
        case cmd_set:
            setValue(command);
            break;
        case cmd_mouse:
            parseMouseCommand(command);
            break;
        case cmd_keyboard:
            parseKeyboardCommand(command);
            break;
        case cmd_open:
            parseOpenCommand(command);
            break;
        default:
            Logger::Instance()->add("Unknown command " + Converter::Instance()->commandToString(command));
            break;
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
        case cmd_connection_connect:
            app->onConnect();
            break;
        case cmd_connection_disconnect:
            app->onDisconnect();
            break;
        case cmd_connection_reachable:
            app->ip = Converter::Instance()->byteToString(*command.data, 3);
            app->onBroadCast(command);
            Logger::Instance()->add(app->ip + " checked reachability");
            break;
        default:
            Logger::Instance()->add("Unknown connection command: " + Converter::Instance()->commandToString(command));
            break;
        }
    }
}

void ApiV3::parseOpenCommand(Command &command)
{
    App *app = Server::Instance()->getApp(command.source);
    if (command.data->length() >= 3 && command.data->at(2) == cmd_action_process)
    {
        QString processString = Converter::Instance()->byteToString(*command.data, 3);
        Logger::Instance()->add("Starting process: " + processString);
        Server::Instance()->startProcess(processString);
    }
    else
    {
        Logger::Instance()->add("Unknown open command: " + Converter::Instance()->commandToString(command));
    }
}

void ApiV3::answerGetRequest(Command &requestCommand)
{
    App *app = Server::Instance()->getApp(requestCommand.source);
    Command *responseCommand = new Command();
    responseCommand->source = Network::Instance()->getServerIp();
    responseCommand->destination = app->ip;
    responseCommand->priority = Command::PRIORITY_HIGH;

    if (requestCommand.data->length() >= 3)
    {
        QByteArray *commandIdentifier = new QByteArray();
        commandIdentifier->append(COMMAND_IDENTIFIER).append(cmd_get).append(requestCommand.data->at(2));

        switch (requestCommand.data->at(2))
        {
        case cmd_get_server_version:
            Logger::Instance()->add("Server version requested");
            responseCommand->data = &commandIdentifier->append(APP_VERSION); // currentVersionCode from update requested
            break;
        case cmd_get_server_name:
            responseCommand->data = &commandIdentifier->append(Server::Instance()->userName.toUtf8());
            break;
        case cmd_get_os_name:
            responseCommand->data = &commandIdentifier->append(Server::Instance()->getServerOs().toUtf8());
            break;
        case cmd_get_api_version:
            responseCommand->data = &commandIdentifier->append(3);
            break;
        case cmd_get_screenshot:
            parseScreenshotProperties(requestCommand);
            answerScreenGetRequest(*responseCommand);
            break;
        case cmd_get_screenshots:
            parseScreenshotProperties(requestCommand);
            Screenshot::Instance()->continueSendingScreenshots = true;
            if (!Screenshot::Instance()->isSendingScreenshot)
            {
                Screenshot::Instance()->keepSendingScreenshots(requestCommand, *responseCommand);
            }
            break;
        default:
            Logger::Instance()->add("Unknown get command: " + Converter::Instance()->commandToString(requestCommand));
            break;
        }
    }

    if (responseCommand->data->length() > 0)
    {
        responseCommand->send();
    }
}

void ApiV3::parseScreenshotProperties(Command &command)
{
    if (command.data->length() >= 4)
    {
        Screenshot::Instance()->lastRequestedWidth = (unsigned char)command.data->at(3) * 100;

        if (command.data->length() >= 5)
        {
            Screenshot::Instance()->lastRequestedQuality = (unsigned char)command.data->at(4);
        }
        else
        {
            Screenshot::Instance()->lastRequestedQuality = Settings::Instance()->screenQualityFull;
        }
    }
    else
    {
        Screenshot::Instance()->lastRequestedWidth = 9999;
    }
}

void ApiV3::answerScreenGetRequest(Command &responseCommand)
{
    QPixmap* screenshotBitmap = Screenshot::Instance()->getResizedScreenshot(Screenshot::Instance()->lastRequestedWidth);
    QByteArray *screenshotData = Converter::Instance()->bitmapToByte(*screenshotBitmap, Screenshot::Instance()->lastRequestedQuality);
    QByteArray *commandIdentifier = new QByteArray();
    commandIdentifier->append(COMMAND_IDENTIFIER).append(cmd_get).append(cmd_get_screenshot);
    commandIdentifier->append(Screenshot::Instance()->lastRequestedWidth / 100);
    responseCommand.data = commandIdentifier;
    responseCommand.data->append(*screenshotData);
    responseCommand.send();
}

void ApiV3::setValue(Command &setCommand)
{
    App *app = Server::Instance()->getApp(setCommand.source);
    if (setCommand.data->length() >= 3)
    {
        switch (setCommand.data->at(2))
        {
        case cmd_set_pin:
            app->pin = Converter::Instance()->byteToString(*setCommand.data, 3);
            Logger::Instance()->add("Pin from " + app->ip + " set to " + app->pin);
            validatePin(*app);
            break;
        case cmd_set_app_version:
            app->appVersion = Converter::Instance()->byteToString(*setCommand.data, 3);
            Logger::Instance()->add("App version from " + app->ip + " set to " + app->appVersion);
            break;
        case cmd_set_app_name:
            app->appName = Converter::Instance()->byteToString(*setCommand.data, 3);
            Logger::Instance()->add("App name from " + app->ip + " set to " + app->appName);
            break;
        case cmd_set_os_version:
            app->osVersion = Converter::Instance()->byteToString(*setCommand.data, 3);
            Logger::Instance()->add("Os version from " + app->ip + " set to " + app->osVersion);
            break;
        case cmd_set_device_name:
            app->deviceName = Converter::Instance()->byteToString(*setCommand.data, 3);
            Logger::Instance()->add("Device name from " + app->ip + " set to " + app->deviceName);
            break;
        default:
            Logger::Instance()->add("Unknown set command: " + Converter::Instance()->commandToString(setCommand));
            break;
        }
    }
}

void ApiV3::parseMouseCommand(Command &command)
{
    if (command.data->length() >= 3)
    {
        switch (command.data->at(2))
        {
        case cmd_mouse_pointers:
            MouseV3::Instance()->parsePointerData(*command.data);
            break;
        case cmd_mouse_pointers_absolute:
            MouseV3::Instance()->parseAbsolutePointerData(*command.data, false);
            break;
        case cmd_mouse_pointers_absolute_presenter:
            MouseV3::Instance()->parseAbsolutePointerData(*command.data, true);
            break;
        case cmd_mouse_pad_action:
            if (command.data->length() >= 4)
            {
                switch (command.data->at(3))
                {
                case cmd_action_down:
                    MouseV3::Instance()->pointersDown();
                    break;
                case cmd_action_up:
                    MouseV3::Instance()->pointersUp();
                    break;
                default:
                    Logger::Instance()->add("Unknown mouse pad command");
                    break;
                }
            }
            break;
        case cmd_mouse_left_action:
            if (command.data->length() >= 4)
            {
                switch (command.data->at(3))
                {
                case cmd_action_down:
                    Logger::Instance()->add("Mouse left down");
                    MouseMac::Instance()->leftMouseDown(false);
                    break;
                case cmd_action_up:
                    Logger::Instance()->add("Mouse left up");
                    MouseMac::Instance()->leftMouseUp(false);
                    break;
                default:
                    Logger::Instance()->add("Unknown mouse left command");
                    break;
                }
            }
            break;
        case cmd_mouse_right_action:
            if (command.data->length() >= 4)
            {
                switch (command.data->at(3))
                {
                case cmd_action_down:
                    Logger::Instance()->add("Mouse right down");
                    MouseMac::Instance()->rightMouseDown();
                    break;
                case cmd_action_up:
                    Logger::Instance()->add("Mouse right up");
                    MouseMac::Instance()->rightMouseUp();
                    break;
                default:
                    Logger::Instance()->add("Unknown mouse right command");
                    break;
                }
            }
            break;
        default:
            Logger::Instance()->add("Unknown mouse command: " + Converter::Instance()->commandToString(command));
            break;
        }
    }
}

void ApiV3::parseKeyboardCommand(Command &command)
{
    if (command.data->length() >= 3)
    {
        switch (command.data->at(2))
        {
        case cmd_keybaord_keycode:
        {
            if (command.data->length() >= 8)
            {
                char action = command.data->at(3);
                unsigned char firstByte = command.data->at(6);
                unsigned char secondByte = command.data->at(7);
                int keyCode = firstByte << 8 | secondByte;

#ifdef Q_OS_MAC
                if (keyCode < KeyboardMac::KEYCODE_C1 || keyCode > KeyboardMac::KEYCODE_C12)
#endif
                {
                    switch (action)
                    {
                    case cmd_action_down:
#ifdef Q_OS_MAC
                        KeyboardMac::Instance()->sendKeyDown(KeyboardMac::Instance()->keycodeToKey(keyCode));
#endif
                        break;
                    case cmd_action_up:
#ifdef Q_OS_MAC
                        KeyboardMac::Instance()->sendKeyUp(KeyboardMac::Instance()->keycodeToKey(keyCode));
#endif
                        break;
                    case cmd_action_click:
#ifdef Q_OS_MAC
                        CGKeyCode keyboardKey = KeyboardMac::Instance()->keycodeToKey(keyCode);
                        if (keyboardKey == KeyboardMac::KEYCODE_UNKOWN)
                        {
                            KeyboardMac::Instance()->sendShortcut(keyboardKey);
                        }
                        else
                        {
                            KeyboardMac::Instance()->sendKeyPress(keyboardKey);
                        }
#endif
                        break;
                    }
                }
                else
                {
#ifdef Q_OS_MAC
                    int actionIndex = keyCode - KeyboardMac::KEYCODE_C1;
#endif
                    Logger::Instance()->add("Received custom key code: " + (actionIndex + 1));

                    if (Settings::Instance()->serialCommands)
                    {
                        Serial::Instance()->sendCommand(command);
                    }
                    else
                    {
                        if (actionIndex > Settings::Instance()->customActions->length() - 1)
                        {
                            Logger::Instance()->add("No custom action set");
                            Server::Instance()->startProcess("http://remote-control-collection.com/help/custom/");
                            Logger::Instance()->trackEvent("Server", "Custom", "Not set");
                        }
                        else
                        {
                            QString path = Settings::Instance()->customActions->at(actionIndex);
                            Server::Instance()->startProcess(path);
                            Logger::Instance()->trackEvent("Server", "Custom", path);
                        }
                    }
                }
            }
            break;
        }
        case cmd_keyboard_string:
        {
            QString keyString = Converter::Instance()->byteToString(*command.data, 3);
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendEachKey(keyString);
#endif
            break;
        }
        default:
        {
            Logger::Instance()->add("Unknown keyboard command: " + Converter::Instance()->commandToString(command));
            break;
        }
        }
    }
}

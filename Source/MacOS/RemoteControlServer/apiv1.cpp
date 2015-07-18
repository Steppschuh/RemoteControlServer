#include "apiv1.h"
#include "app.h"
#include "logger.h"
#include "server.h"

ApiV1* ApiV1::instance = NULL;

ApiV1* ApiV1::Instance()
{
    if (!instance)
    {
        instance = new ApiV1();
    }
    return instance;
}

ApiV1::ApiV1():
    cmd_info_device_name("[cmd_15]"),
    cmd_info_device_osversion("[cmd_16]"),
    cmd_info_app_name("[cmd_17]"),
    cmd_info_app_version("[cmd_18]"),
    cmd_info_app_mode("[cmd_19]"),
    cmd_connect("[cmd_connect]"),
    cmd_disconnect("[cmd_disconnect]"),
    cmd_lizens("[cmd_lizens]"),
    cmd_version("[cmd_version]"),
    cmd_mouse_string("[cmd_mouse]"),
    cmd_keyboard_string("[cmd_keyboard]"),
    cmd_media_string("[cmd_music]"),
    cmd_slideshow_string("[cmd_slide]"),
    cmd_process_string("[cmd_process]"),
    cmd_scroll_string("[cmd_scroll]"),
    cmd_broadcast_string("[cmd_broadcast]"),
    cmd_google_string("[cmd_google]"),
    cmd_shortcut_string("[cmd_short]")
{
    readableCommand = "Unknown";
}

bool ApiV1::isBroadcast(Command &command)
{
    return (command.dataAsString().contains(cmd_broadcast_string)) ? true : false;
}

void ApiV1::parseCommand(Command &command)
{
    QString cmd = command.dataAsString();
    QString value = getCommandValue(cmd);
    App *app = Server::Instance()->getApp(command.source);

    readableCommand = "Unknown";
    if (cmd.contains(cmd_mouse_string))
    {
        parseMouseCommand(cmd);
        Logger::Instance()->add("Mouse: " + value);
    }
    else if (cmd.contains(cmd_keyboard_string))
    {
        parseKeyboardCommand(cmd);
        Logger::Instance()->add("Keyboard: " + readableCommand);
    }
    else if (cmd.contains(cmd_media_string))
    {
        if (app->license->isProversion)
        {
            parseMediaCommand(cmd);
        }
        else
        {
            app->license->requestUpgrade();
        }
        Logger::Instance()->add("Media: " + readableCommand);
    }
    else if (cmd.contains(cmd_slideshow_string))
    {
        if (app->license->isProversion)
        {
            parseSlideshowCommand(cmd);
        }
        else
        {
            app->license->requestUpgrade();
        }
        Logger::Instance()->add("Slideshow: " + readableCommand);
    }
    else if (cmd.contains(cmd_scroll_string))
    {
        parseScrollCommand(cmd);
        Logger::Instance()->add("Scroll: " + readableCommand);
    }
    else if (cmd.contains(cmd_shortcut_string))
    {
        parseShortcutCommand(cmd);
        Logger::Instance()->add("Shortcut: " + readableCommand);
    }
    else if (cmd.contains(cmd_process_string))
    {
        //Process.start(value)
        Logger::Instance()->add("Process: " + value);
    }
    else if (cmd.contains(cmd_google_string))
    {
        //Process.Start("http://google.com/search?q=" + value)
        Logger::Instance()->add("Google: " + value);
    }
    else if (cmd.contains(cmd_broadcast_string))
    {
        command.source = value;
        app = Server::Instance()->getApp(value);
        app->onBroadCast(command);
    }
    else if (cmd.contains(cmd_connect))
    {
        app->deviceName = value;
        Logger::Instance()->add("Device name set to " + value);
        app->onConnect();
    }
    else if (cmd.contains(cmd_disconnect))
    {
        app = Server::Instance()->getApp(value);
        app->onDisconnect();
    }
    else if (cmd.contains(cmd_info_device_name))
    {
        app->deviceName = value;
        Logger::Instance()->add("Device name set to " + value);
    }
    else if (cmd.contains(cmd_info_device_osversion))
    {
        app->osVersion = value;
        app->detectOs();
        Logger::Instance()->add("Device OS version set to " + value);
    }
    else if (cmd.contains(cmd_info_app_name))
    {
        app->appName = value;
        Logger::Instance()->add("App name set to " + value);
    }
    else if (cmd.contains(cmd_info_app_version))
    {
        app->appVersion = value;
        Logger::Instance()->add("App version set to " + value);
    }
    else if (cmd.contains(cmd_info_app_mode) || cmd.contains(cmd_lizens))
    {
        License* license= new License();
        license->isProversion = (value.toLower() == "pro" || value.toLower() == "trial")
                ? true : false;
        app->license = license;
        Logger::Instance()->add("App license set to " + value);
    }
    else
    {
        Logger::Instance()->add("Unknown ApiV1: " + cmd);
    }
}

QString ApiV1::getCommandValue(QString cmd)
{
    return cmd.right(cmd.length() - cmd.indexOf("]") - 1);
}

void ApiV1::parseMouseCommand(QString cmd)
{

}

void ApiV1::parseKeyboardCommand(QString cmd)
{

}

void ApiV1::parseMediaCommand(QString cmd)
{

}

void ApiV1::parseScrollCommand(QString cmd)
{

}

void ApiV1::parseShortcutCommand(QString cmd)
{

}

void ApiV1::parseSlideshowCommand(QString cmd)
{

}

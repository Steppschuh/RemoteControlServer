#include "apiv1.h"
#include "app.h"
#include "logger.h"
#include "media.h"
#include "mousev2.h"
#include "serial.h"
#include "server.h"
#include "settings.h"

#ifdef Q_OS_MAC
    #include "keyboardmac.h"
    #include "mousemac.h"

    #include <Carbon/Carbon.h>
#endif

#include <QDesktopServices>

#include <QDebug>

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
    if (cmd.contains(cmd_keyboard_string))
    {
        parseKeyboardCommand(cmd);
        Logger::Instance()->add("Keyboard: " + readableCommand);
    }
    else if (cmd.contains(cmd_media_string))
    {
        parseMediaCommand(cmd);
        Logger::Instance()->add("Media: " + readableCommand);
    }
    else if (cmd.contains(cmd_slideshow_string))
    {
        parseSlideshowCommand(cmd);
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
        Server::Instance()->startProcess(value);
        Logger::Instance()->add("Process: " + value);
    }
    else if (cmd.contains(cmd_google_string))
    {
        QDesktopServices::openUrl("http://google.com/search?q=" + value);
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
        Logger::Instance()->add("Device OS version set to " + value);
    }
    else if (cmd.contains(cmd_info_app_name))
    {
        app->appName = value;
        Logger::Instance()->add("App name set to " + value);
    }
    else if (cmd.contains(cmd_info_app_version) || cmd.contains(cmd_version))
    {
        app->appVersion = value;
        Logger::Instance()->add("App version set to " + value);
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

//void ApiV1::parseMouseCommand(QString cmd)
//{
//    MouseV2::Instance()->parseMouse(cmd);
//}

void ApiV1::parseKeyboardCommand(QString cmd)
{
    if (cmd.contains("<") && cmd.contains(">"))
    {
        if (cmd.contains("<Enter>"))
        {
            readableCommand = "Enter";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_ENTER));
#endif
            return;
        }
        else if (cmd.contains("<Back>"))
        {
            readableCommand = "Back";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_BACK));
#endif
            return;
        }
        else if (cmd.contains("<tab>"))
        {
            readableCommand = "Tab";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_TAB));
#endif
            return;
        }
        else if (cmd.contains("<caps>"))
        {
            readableCommand = "Capslock";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_CAPS_LOCK));
#endif
            return;
        }
        else if (cmd.contains("<ctrl>"))
        {
            readableCommand = "Control";
            if (cmd.contains("down"))
            {
#ifdef Q_OS_MAC
                KeyboardMac::Instance()->sendKeyDown(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_CONTROL));
#endif
            }
            else
            {
#ifdef Q_OS_MAC
                KeyboardMac::Instance()->sendKeyUp(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_CONTROL));
#endif
            }
            return;
        }
        else if (cmd.contains("<alt>"))
        {
            readableCommand = "Alt";
            if (cmd.contains("down"))
            {
#ifdef Q_OS_MAC
                KeyboardMac::Instance()->sendKeyDown(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_ALT));
#endif
            }
            else
            {
#ifdef Q_OS_MAC
                KeyboardMac::Instance()->sendKeyUp(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_ALT));
#endif
            }
            return;
        }
        else if (cmd.contains("<shift>"))
        {
            readableCommand = "Shift";
            if (cmd.contains("down"))
            {
#ifdef Q_OS_MAC
                KeyboardMac::Instance()->sendKeyDown(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_SHIFT));
#endif
            }
            else
            {
#ifdef Q_OS_MAC
                KeyboardMac::Instance()->sendKeyUp(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_SHIFT));
#endif
            }
            return;
        }
        else if (cmd.contains("<del>"))
        {
            readableCommand = "Delete";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_DEL));
#endif
            return;
        }
        else if (cmd.contains("<win>"))
        {
            readableCommand = "Windows";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_WINDOWS));
#endif
            return;
        }
        else if (cmd.contains("<esc>"))
        {
            readableCommand = "Escape";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_ESCAPE));
#endif
            return;
        }
        else if (cmd.contains("<end>"))
        {
            readableCommand = "End";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_MOVE_END));
#endif
            return;
        }
        else if (cmd.contains("<ins>"))
        {
            readableCommand = "Insert";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_INSERT));
#endif
            return;
        }
        else if (cmd.contains("<up>"))
        {
            readableCommand = "Up";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_UP));
#endif
            return;
        }
        else if (cmd.contains("<down>"))
        {
            readableCommand = "Down";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_DOWN));
#endif
            return;
        }
        else if (cmd.contains("<left>"))
        {
            readableCommand = "Left";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_LEFT));
#endif
            return;
        }
        else if (cmd.contains("<right>"))
        {
            readableCommand = "Right";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_RIGHT));
#endif
            return;
        }
        else if (cmd.contains("<space>"))
        {
            readableCommand = "Space";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_SPACE));
#endif
            return;
        }
        else
        {
            if (Settings::Instance()->serialCommands)
            {
                readableCommand = getCommandValue(cmd);
                Command *command = new Command();
                if (readableCommand.contains("<0>"))
                {
                    command->data->append(char(0));
                }
                else if (readableCommand.contains("<1>"))
                {
                    command->data->append(1);
                }
                else if (readableCommand.contains("<2>"))
                {
                    command->data->append(2);
                }
                else
                {
                    command->data->append(char(0));
                }
                Serial::Instance()->sendCommand(*command);
            }
            return;
        }
    }
    else
    {
        readableCommand  = getCommandValue(cmd);
        if (readableCommand.toLower().contains("execute") && Settings::Instance()->serialCommands)
        {
            Command *command  = new Command();
            if (readableCommand.contains("10"))
            {
                command->data->append(char(0));
            }
            else if (readableCommand.contains("11"))
            {
                command->data->append(1);
            }
            else if (readableCommand.contains("12"))
            {
                command->data->append(2);
            }
            else
            {
                command->data->append(char(0));
            }
            Serial::Instance()->sendCommand(*command);
        }
        else if (readableCommand == " ")
        {
            readableCommand = "Space";
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_SPACE));
#endif
        }
        else
        {
#ifdef Q_OS_MAC
            KeyboardMac::Instance()->sendEachKey(readableCommand);
#endif
        }
    }
}

void ApiV1::parseMediaCommand(QString cmd)
{
    QString value = getCommandValue(cmd);
    if (value == "play")
    {
        readableCommand = "Play";
        Media::Instance()->playMedia();
    }
    else if (value == "stop")
    {
        readableCommand = "Stop";
        Media::Instance()->stopMedia();
    }
    else if (value == "next")
    {
        readableCommand = "Next";
        Media::Instance()->nextMedia();
    }
    else if (value == "prev")
    {
        readableCommand = "Previous";
        Media::Instance()->previousMedia();
    }
    else if (value == "volup")
    {
        readableCommand = "Volume up";
        Media::Instance()->volumeUp();
    }
    else if (value == "voldown")
    {
        readableCommand = "Volume down";
        Media::Instance()->volumeDown();
    }
    else if (value == "mute")
    {
        readableCommand = "Mute";
        Media::Instance()->volumeMute();
    }
    else if (value == "launch")
    {
        readableCommand = "Launch player";
        Media::Instance()->launchPlayer();
    }
}

void ApiV1::parseScrollCommand(QString cmd)
{
    QString value = getCommandValue(cmd);
    if (value == "zoomin")
    {
        readableCommand = "Zoom in";
        MouseMac::Instance()->zoom(1,1);
    }
    else if (value == "zoomout")
    {
        readableCommand = "Zoom out";
        MouseMac::Instance()->zoom(-1,1);
    }
    else if (value == "back")
    {
        readableCommand = "Back";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_BROWSER_BACK);
#endif
    }
    else if (value == "forward")
    {
        readableCommand = "Forward";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_BROWSER_FORWARD);
#endif
    }
    else if (value == "pageup")
    {
        readableCommand = "Page up";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_PAGE_UP));
#endif
    }
    else if (value == "pagedown")
    {
        readableCommand = "Page down";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_PAGE_DOWN));
#endif
    }
    else if (value == "cancel")
    {
        readableCommand = "Cancel";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_CANCEL);
#endif
    }
    else if (value == "refresh")
    {
        readableCommand = "Refresh";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_REFRESH);
#endif
    }
    else if (value == "fullexit")
    {
        readableCommand = "Exit fullscreen";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_FULLSCREEN);
#endif
    }
    else if (value == "fullscreen")
    {
        readableCommand = "Fullscreen";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_FULLSCREEN);
#endif
    }

}

void ApiV1::parseShortcutCommand(QString cmd)
{
    QString value = getCommandValue(cmd);
    if (value == "desktop")
    {
        readableCommand = "Show desktop";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_SHOW_DESKTOP);
#endif
    }
    else if (value == "close")
    {
        readableCommand = "Close";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_CLOSE);
#endif
    }
    else if (value == "copy")
    {
        readableCommand = "Copy";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_COPY);
#endif
    }
    else if (value == "paste")
    {
        readableCommand = "Paste";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_PASTE);
#endif
    }
    else if (value == "selectall")
    {
        readableCommand = "Select all";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_SELECT_ALL);
#endif
    }
    else if (value == "undo")
    {
        readableCommand = "Undo";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendShortcut(KeyboardMac::KEYCODE_UNDO);
#endif
    }
    else if (value == "standby")
    {
        readableCommand = "Standby";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->standby();
#endif
    }
    else if (value == "shutdown")
    {
        readableCommand = "Shutdown";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->shutdown();
#endif
    }
}

void ApiV1::parseSlideshowCommand(QString cmd)
{
    QString value = getCommandValue(cmd);
    if (value == "pause")
    {
        readableCommand = "Pause slideshow";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendKeyPress(kVK_ANSI_B);
#endif
    }
    else if (value == "next")
    {
        readableCommand = "Next slide";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_RIGHT));
#endif
    }
    else if (value == "prev")
    {
        readableCommand = "Previous slide";
#ifdef Q_OS_MAC
        KeyboardMac::Instance()->sendKeyPress(KeyboardMac::Instance()->keycodeToKey(KeyboardMac::KEYCODE_LEFT));
#endif
    }
}

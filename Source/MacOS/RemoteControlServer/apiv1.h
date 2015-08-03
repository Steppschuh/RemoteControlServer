#ifndef APIV1_H
#define APIV1_H

#include "command.h"

#include <QString>

class ApiV1
{
public:
    static ApiV1* Instance();

    bool isBroadcast(Command &command);
    void parseCommand(Command &command);

private:
    static ApiV1* instance;
    ApiV1();

    const QString cmd_info_device_name;
    const QString cmd_info_device_osversion;
    const QString cmd_info_app_name;
    const QString cmd_info_app_version;
    const QString cmd_info_app_mode;

    const QString cmd_connect;
    const QString cmd_disconnect;
    const QString cmd_lizens;
    const QString cmd_version;

    const QString cmd_mouse_string;
    const QString cmd_keyboard_string;
    const QString cmd_media_string;
    const QString cmd_slideshow_string;
    const QString cmd_process_string;
    const QString cmd_scroll_string;
    const QString cmd_broadcast_string;
    const QString cmd_google_string;
    const QString cmd_shortcut_string;

    QString readableCommand;

    QString getCommandValue(QString cmd);
    void parseMouseCommand(QString cmd);
    void parseKeyboardCommand(QString cmd);
    void parseMediaCommand(QString cmd);
    void parseScrollCommand(QString cmd);
    void parseShortcutCommand(QString cmd);
    void parseSlideshowCommand(QString cmd);
};

#endif // APIV1_H

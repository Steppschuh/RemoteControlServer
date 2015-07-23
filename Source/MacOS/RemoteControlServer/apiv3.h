#ifndef APIV3_H
#define APIV3_H

#include "app.h"
#include "command.h"

class ApiV3
{
public:
    static ApiV3 *Instance();

    const char COMMAND_IDENTIFIER;

    bool isBroadcast(Command &command);
    bool isConnectionCommand(Command &command);
    void requestPin(App &app);
    void answerBroadCast(App &app);
    void parseCommand(Command &command);

private:
    ApiV3();
    static ApiV3 *instance;

    enum{
        cmd_connection = 10,
        cmd_broadcast = 16,
        cmd_set = 17,
        cmd_open = 18,
        cmd_get = 19,

        cmd_connection_reachable = 0,
        cmd_connection_protected = 1,
        cmd_connection_connect = 2,
        cmd_connection_disconnect = 3,

        cmd_action_process = 3,

        cmd_set_pin = 0,
        cmd_set_app_version = 1,
        cmd_set_app_name = 2,
        cmd_set_os_version = 3,
        cmd_set_device_name = 5,

        cmd_get_server_version = 1,
        cmd_get_server_name = 2,
        cmd_get_os_name = 4,
        cmd_get_screenshot = 5,
        cmd_get_api_version = 6,
        cmd_get_screenshots = 7,

        cmd_mouse = 20,

        cmd_keyboard = 21
    };
//    const char cmd_disconnect;
//    const char cmd_pause;
//    const char cmd_resume;
//    const char cmd_control;
//    const char cmd_pin;
//    const char cmd_broadcast;
//    const char cmd_set;
//    const char cmd_open;

//    const char cmd_connection_protected;
//    const char cmd_connection_connect;
//    const char cmd_connection_disconnect;

//    const char cmd_action_up;
//    const char cmd_action_down;
//    const char cmd_action_click;
//    const char cmd_action_process;

//    const char cmd_set_pin;
//    const char cmd_set_app_version;
//    const char cmd_set_app_name;
//    const char cmd_set_os_version;
//    const char cmd_set_device_name;

//    const char cmd_get_server_version;
//    const char cmd_get_os_name;
//    const char cmd_get_screenshot;
//    const char cmd_get_screenshots;
//    const char cmd_get_api_version;

//    const char cmd_mouse;
//    const char cmd_mouse_pointers;
//    const char cmd_mouse_pointers_absolute;
//    const char cmd_mouse_pointers_absolute_presenter;
//    const char cmd_mouse_pad_action;
//    const char cmd_mouse_left_action;
//    const char cmd_mouse_right_action;

//    const char cmd_keyboard;
//    const char cmd_keyboard_unicode;
//    const char cmd_keyboard_string;
//    const char cmd_keyboard_keycode;

    void validatePin(App &app);
    void parseConnectCommand(Command &command);
    void parseOpenCommand(Command &command);
    void answerGetRequest(Command &requestCommand);
//    void parseScrenshotProperties(Command *command);
//    void answerScreenGetRequest(Command *requestCommand, Command *responseCommand);
    void setValue(Command &setCommand);
//    void parseMouseCommand(Command *command);
//    void parseKeyboardCommand(Command *command);
};

#endif // APIV3_H

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

        cmd_action_up = 0,
        cmd_action_down = 1,
        cmd_action_click = 2,
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
        cmd_mouse_pointers = 1,
        cmd_mouse_pointers_absolute = 0,
        cmd_mouse_pointers_absolute_presenter = 5,
        cmd_mouse_pad_action = 2,
        cmd_mouse_left_action = 3,
        cmd_mouse_right_action = 4,

        cmd_keyboard = 21,

        cmd_keyboard_string = 1,
        cmd_keybaord_keycode = 2
    };

    void validatePin(App &app);
    void parseConnectCommand(Command &command);
    void parseOpenCommand(Command &command);
    void answerGetRequest(Command &requestCommand);
    void parseScreenshotProperties(Command &command);
    void answerScreenGetRequest(Command &responseCommand);
    void setValue(Command &setCommand);
    void parseMouseCommand(Command &command);
    void parseKeyboardCommand(Command &command);
};

#endif // APIV3_H

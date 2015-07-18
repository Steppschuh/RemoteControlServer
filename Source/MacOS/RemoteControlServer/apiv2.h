#ifndef APIV2_H
#define APIV2_H

#include <QString>

#include "app.h"
#include "command.h"

class ApiV2
{
public:
    static ApiV2 *Instance();

    const char COMMAND_IDENTIFIER;

    bool isBroadcast(Command &command);
    void answerBroadCast(App &app);
    void parseCommand(Command &command);

private:
    ApiV2();
    static ApiV2 *instance;

    enum{
        cmd_connect = 10,
        cmd_disconnect = 11,
        cmd_pause = 12,
        cmd_resume = 13,
        cmd_control = 14,
        cmd_pin = 15,
        cmd_broadcast = 16,

        cmd_media = 20,
        cmd_mouse = 21,
        cmd_pointer = 22,
        cmd_slideshow = 23,
        cmd_motion = 24,
        cmd_keyboard = 25,
        cmd_scroll = 26,
        cmd_request = 27,
        cmd_laser = 28,
        cmd_custom = 29,

//    const char cmd_media_play;
//    const char cmd_media_stop;
//    const char cmd_media_prev;
//    const char cmd_media_next;
//    const char cmd_media_volup;
//    const char cmd_media_voldown;
//    const char cmd_media_volmute;
//    const char cmd_media_launch;

//    const char cmd_mouse_down;
//    const char cmd_mouse_up;
//    const char cmd_mouse_down_2;
//    const char cmd_mouse_up_2;
//    const char cmd_mouse_move;
//    const char cmd_mouse_left_down;
//    const char cmd_mouse_left_up;
//    const char cmd_mouse_right_down;
//    const char cmd_mouse_right_up;
//    const char cmd_mouse_scroll;
//    const char cmd_mouse_set;
//    const char cmd_mouse_left_click;
//    const char cmd_mouse_right_click;

//    const char cmd_pointer_start;
//    const char cmd_pointer_end;
//    const char cmd_pointer_move;
//    const char cmd_pointer_calibrate;

//    const char cmd_request_screen;
//    const char cmd_request_screen_full;
//    const char cmd_request_screen_next;
//    const char cmd_request_connect;
        cmd_request_pin = 4
    };

//    QString readableCommand;

    void requestPin(App &app);
//    void refuseBroadCast(App &app);
    void parseGeneralCommand(Command &command);
    void parseRemoteCommand(Command &command);
//    void parseMouseCommand(Command &command);
//    void parsePointerCommand(Command &command);
//    void parseLaserCommand(Command &command);
//    void parseMediaCommand(Command &command);
//    void parseRequest(Command &command);
//    void parseCustomCommand(Command &command);
};

#endif // APIV2_H

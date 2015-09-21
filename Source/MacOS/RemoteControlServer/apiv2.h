#ifndef APIV2_H
#define APIV2_H

#include <QObject>
#include <QString>

#include "app.h"
#include "command.h"

class ApiV2 : public QObject
{
    Q_OBJECT

public:
    static ApiV2 *Instance();

    const char COMMAND_IDENTIFIER;

    bool isBroadcast(Command &command);
    void requestPin(App &app);
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

        cmd_media_play = 0,
        cmd_media_stop = 1,
        cmd_media_prev = 2,
        cmd_media_next = 3,
        cmd_media_volup = 4,
        cmd_media_voldown = 5,
        cmd_media_volmute = 6,
        cmd_media_launch = 7,

        cmd_mouse_down = 0,
        cmd_mouse_up = 1,
        cmd_mouse_down_2 = 2,
        cmd_mouse_up_2 = 3,
        cmd_mouse_move = 4,
        cmd_mouse_left_down = 5,
        cmd_mouse_left_up = 6,
        cmd_mouse_right_down = 7,
        cmd_mouse_right_up = 8,
        cmd_mouse_scroll = 9,
        cmd_mouse_set = 10,
        cmd_mouse_left_click = 11,
        cmd_mouse_right_click = 12,

        cmd_pointer_start = 0,
        cmd_pointer_end = 1,
        cmd_pointer_move = 2,
        cmd_pointer_calibrate = 3,

        cmd_request_screen = 0,
        cmd_request_screen_full = 1,
        cmd_request_screen_next = 2,
        cmd_request_connect = 3,
        cmd_request_pin = 4
    };

    QString readableCommand;

    void parseGeneralCommand(Command &command);
    void parseRemoteCommand(Command &command);
    void parseMouseCommand(Command &command);
    void parsePointerCommand(Command &command);
    void parseLaserCommand(Command &command);
    void parseMediaCommand(Command &command);
    void parseRequest(Command &command);
    void parseCustomCommand(Command &command);
};

#endif // APIV2_H

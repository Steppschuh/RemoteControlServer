#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <ApplicationServices/ApplicationServices.h>
#include <QString>

class Keyboard
{
public:
    static Keyboard* Instance();

    enum
    {
        KEYCODE_UNKOWN = 0,
        KEYCODE_BACK = 4,
        KEYCODE_UP = 19,
        KEYCODE_DOWN = 20,
        KEYCODE_LEFT = 21,
        KEYCODE_RIGHT = 22,
        KEYCODE_ALT = 57,
        KEYCODE_SHIFT = 59,
        KEYCODE_CAPS_LOCK = 115,
        KEYCODE_DEL = 67,
        KEYCODE_ENTER = 66,
        KEYCODE_ESCAPE = 111,
        KEYCODE_DEL_FORWARD = 112,
        KEYCODE_CONTROL = 113,
        KEYCODE_INSERT = 124,
        KEYCODE_MOVE_END = 123,
        KEYCODE_MOVE_HOME = 122,
        KEYCODE_WINDOWS = 171,
        KEYCODE_PAGE_DOWN = 93,
        KEYCODE_PAGE_UP = 92,
        KEYCODE_SPACE = 62,
        KEYCODE_TAB = 61,

        KEYCODE_F1 = 131,
        KEYCODE_F2 = 132,
        KEYCODE_F3 = 133,
        KEYCODE_F4 = 134,
        KEYCODE_F5 = 135,
        KEYCODE_F6 = 136,
        KEYCODE_F7 = 137,
        KEYCODE_F8 = 138,
        KEYCODE_F9 = 139,
        KEYCODE_F10 = 140,
        KEYCODE_F11 = 141,
        KEYCODE_F12 = 142,

        KEYCODE_C1 = 221,
        KEYCODE_C2 = 222,
        KEYCODE_C3 = 223,
        KEYCODE_C4 = 224,
        KEYCODE_C5 = 225,
        KEYCODE_C6 = 226,
        KEYCODE_C7 = 227,
        KEYCODE_C8 = 228,
        KEYCODE_C9 = 229,
        KEYCODE_C10 = 230,
        KEYCODE_C11 = 231,
        KEYCODE_C12 = 232,

        KEYCODE_COPY = 201,
        KEYCODE_PASTE = 202,
        KEYCODE_SELECT_ALL = 203,
        KEYCODE_CUT = 204,
        KEYCODE_SHOW_DESKTOP = 205,
        KEYCODE_ZOOM_IN = 206,
        KEYCODE_ZOOM_OUT = 207,
        KEYCODE_CLOSE = 208,
        KEYCODE_SHUTDOWN = 209,
        KEYCODE_STANDBY = 210
    };

    void sendKeyPress(CGKeyCode key);
    void sendKeyDown(CGKeyCode key);
    void sendKeyUp(CGKeyCode key);
    void sendEachKey(QString message);
    void keycodeToShortcut(int keyCode);
    CGKeyCode keycodeToKey(int keyCode);

private:
    static Keyboard* instance;
    Keyboard();

    void sendUnicodeKeyPress(QChar character);
    CFStringRef createStringForKey(CGKeyCode keyCode);
};

#endif // KEYBOARD_H

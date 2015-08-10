#ifndef KEYBOARDMAC_H
#define KEYBOARDMAC_H

#include <limits>

#include <ApplicationServices/ApplicationServices.h>
#include <QString>

class KeyboardMac
{
public:
    static KeyboardMac* Instance();

    enum
    {
        KEYCODE_UNKOWN = std::numeric_limits<CGKeyCode>::max(),
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
        KEYCODE_COMMAND = 114,
        KEYCODE_FUNCTION = 110,
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
        KEYCODE_STANDBY = 210,
        KEYCODE_CANCEL = 211,
        KEYCODE_REFRESH = 212,
        KEYCODE_FULLSCREEN = 213,
        KEYCODE_UNDO = 214,
        KEYCODE_BROWSER_BACK = 215,
        KEYCODE_BROWSER_FORWARD = 216
    };

    void sendKeyPress(CGKeyCode key);
    void sendKeyDown(CGKeyCode key, UniChar c = NULL);
    void sendKeyUp(CGKeyCode key, UniChar c = NULL);
    void sendEachKey(QString message);
    void sendShortcut(int keyCode);
    CGKeyCode keycodeToKey(int keyCode);
    void standby();
    void shutdown();

private:
    static KeyboardMac* instance;
    KeyboardMac();

    bool shiftDown;
    bool fnDown;
    bool ctrlDown;
    bool altDown;
    bool cmdDown;

    CFStringRef createStringForKey(CGKeyCode keyCode);
    void sendUnicodeKeyPress(QChar character);
};

#endif // KEYBOARDMAC_H

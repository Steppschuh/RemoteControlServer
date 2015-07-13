#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QString>

class Keyboard
{
public:
    Keyboard *Instance();

private:
    Keyboard();
    Keyboard *instance;

//    int KEYCODE_UNKOWN;
//    int KEYCODE_BACK;
//    int KEYCODE_UP;
//    int KEYCODE_DOWN;
//    int KEYCODE_LEFT;
//    int KEYCODE_RIGHT;
//    int KEYCODE_ALT;
//    int KEYCODE_SHIFT;
//    int KEYCODE_CAPS_LOCK;
//    int KEYCODE_DEL;
//    int KEYCODE_ENTER;
//    int KEYCODE_ESCAPE;
//    int KEYCODE_DEL_FORWARD;
//    int KEYCODE_CONTROL;
//    int KEYCODE_INSERT;
//    int KEYCODE_MOVE_END;
//    int KEYCODE_MOVE_HOME;
//    int KEYCODE_WINDOWS;
//    int KEYCODE_PAGE_DOWN;
//    int KEYCODE_PAGE_UP;
//    int KEYCODE_SPACE;
//    int KEYCODE_TAB;
    
//    int KEYCODE_F1;
//    int KEYCODE_F2;
//    int KEYCODE_F3;
//    int KEYCODE_F4;
//    int KEYCODE_F5;
//    int KEYCODE_F6;
//    int KEYCODE_F7;
//    int KEYCODE_F8;
//    int KEYCODE_F9;
//    int KEYCODE_F10;
//    int KEYCODE_F11;
//    int KEYCODE_F12;

//    int KEYCODE_C1;
//    int KEYCODE_C2;
//    int KEYCODE_C3;
//    int KEYCODE_C4;
//    int KEYCODE_C5;
//    int KEYCODE_C6;
//    int KEYCODE_C7;
//    int KEYCODE_C8;
//    int KEYCODE_C9;
//    int KEYCODE_C10;
//    int KEYCODE_C11;
//    int KEYCODE_C12;

//    int KEYCODE_COPY;
//    int KEYCODE_PASTE;
//    int KEYCODE_SELECT_ALL;
//    int KEYCODE_CUT;
//    int KEYCODE_SHOW_DESKTOP;
//    int KEYCODE_ZOOM_IN;
//    int KEYCODE_ZOOM_OUT;
//    int KEYCODE_CLOSE;
//    int KEYCODE_SHUTDOWN;
//    int KEYCODE_STANDBY;

//    void sendKeyPress(!!);
//    void sendUnicodeKeyPress(char character);
//    void sendKeyDown(!!);
//    void sendKeyUp(!!);
//    void sendEachKey(QString message);
//    void sendShortcut(!!);
//    !! stringToKey(QString key);
//    void keycodeToShortcut(int keycode);
//    !! keycodeToKey(int keycode);
//    void hibernate();
//    void standby();
//    void shutdown();
//    void restart();

};

#endif // KEYBOARD_H

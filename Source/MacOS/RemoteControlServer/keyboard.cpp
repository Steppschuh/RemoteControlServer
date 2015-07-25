#include "keyboard.h"

#include <QObject>

#include <Carbon/Carbon.h>

#include <QDebug>

Keyboard *Keyboard::instance = NULL;

Keyboard *Keyboard::Instance()
{
    if (!instance)
    {
        instance = new Keyboard();
    }
    return instance;
}

Keyboard::Keyboard()
{
}

void Keyboard::sendKeyPress(CGKeyCode key)
{
    if (key)
    {
        sendKeyDown(key);
        sendKeyUp(key);
    }
}

void Keyboard::sendUnicodeKeyPress(QChar character)
{
    UniChar c = (unsigned char) character.toLatin1();
    CGEventRef keyEvent = CGEventCreateKeyboardEvent(NULL, 0, true);
    CGEventKeyboardSetUnicodeString(keyEvent, 1, &c);
    CGEventPost(kCGSessionEventTap, keyEvent);
    CFRelease(keyEvent);

    keyEvent = CGEventCreateKeyboardEvent(NULL, 0, false);
    CGEventKeyboardSetUnicodeString(keyEvent, 1, &c);
    CGEventPost(kCGSessionEventTap, keyEvent);
    CFRelease(keyEvent);
}

void Keyboard::sendKeyDown(CGKeyCode key)
{
    CGEventRef command = CGEventCreateKeyboardEvent(NULL, key, true);
    CGEventPost(kCGAnnotatedSessionEventTap, command);
    CFRelease(command);
}

void Keyboard::sendKeyUp(CGKeyCode key)
{
    CGEventRef command = CGEventCreateKeyboardEvent(NULL, key, false);
    CGEventPost(kCGAnnotatedSessionEventTap, command);
    CFRelease(command);
}

void Keyboard::sendEachKey(QString message)
{
    for (int i = 0; i < message.length(); ++i)
    {
        QChar character = message.at(i);
        sendUnicodeKeyPress(character);
    }
}

void Keyboard::keycodeToShortcut(int keyCode)
{

}

CGKeyCode Keyboard::keycodeToKey(int keyCode)
{
    switch (keyCode)
    {
    case KEYCODE_BACK:
        return kVK_Delete;
    case KEYCODE_CAPS_LOCK:
        return kVK_CapsLock;
    case KEYCODE_DEL:
        return kVK_Delete;
    case KEYCODE_ENTER:
        return kVK_Return;
    case KEYCODE_ESCAPE:
        return kVK_Escape;
//    case KEYCODE_INSERT:
//        return Keys.Insert
//    case KEYCODE_MOVE_END
//        return Keys.End
//    case KEYCODE_MOVE_HOME
//        return Keys.Home
//    case KEYCODE_PAGE_DOWN
//        return Keys.PageDown
//    case KEYCODE_PAGE_UP
//        return Keys.PageUp
    case KEYCODE_SPACE:
        return kVK_Space;
    case KEYCODE_TAB:
        return kVK_Tab;
    case KEYCODE_UP:
        return kVK_UpArrow;
    case KEYCODE_DOWN:
        return kVK_DownArrow;
    case KEYCODE_LEFT:
        return kVK_LeftArrow;
    case KEYCODE_RIGHT:
        return kVK_RightArrow;
    case KEYCODE_ALT:
        return kVK_Option;
    case KEYCODE_CONTROL:
        return kVK_Control;
    case KEYCODE_SHIFT:
        return kVK_Shift;
    case KEYCODE_DEL_FORWARD:
        return kVK_ForwardDelete;
//    case KEYCODE_WINDOWS
//        return Keys.LWin
    case KEYCODE_F1:
        return kVK_F1;
    case KEYCODE_F2:
        return kVK_F2;
    case KEYCODE_F3:
        return kVK_F3;
    case KEYCODE_F4:
        return kVK_F4;
    case KEYCODE_F5:
        return kVK_F5;
    case KEYCODE_F6:
        return kVK_F6;
    case KEYCODE_F7:
        return kVK_F7;
    case KEYCODE_F8:
        return kVK_F8;
    case KEYCODE_F9:
        return kVK_F9;
    case KEYCODE_F10:
        return kVK_F10;
    case KEYCODE_F11:
        return kVK_F11;
    case KEYCODE_F12:
        return kVK_F12;
    default:
        return NULL;
    }
}



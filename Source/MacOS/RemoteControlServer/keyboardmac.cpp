#include "keyboardmac.h"
#include "logger.h"

#include <QObject>

#include <Carbon/Carbon.h>

//#include <ApplicationServices/ApplicationServices.h>
//#include <IOKit/hidsystem/ev_keymap.h>
//#include "macx.h"

#include <QDebug>

KeyboardMac *KeyboardMac::instance = NULL;

KeyboardMac *KeyboardMac::Instance()
{
    if (!instance)
    {
        instance = new KeyboardMac();
    }
    return instance;
}

KeyboardMac::KeyboardMac()
{
}

void KeyboardMac::sendKeyPress(CGKeyCode key)
{
    if (key)
    {
        sendKeyDown(key);
        sendKeyUp(key);
    }
}

void KeyboardMac::sendUnicodeKeyPress(QChar character)
{
    UniChar c = (unsigned char) character.toLatin1();
    sendKeyDown(-1, c);
    sendKeyUp(-1, c);
}

void KeyboardMac::sendKeyDown(CGKeyCode key, UniChar c)
{
    if (key == kVK_Shift)
    {
        shiftDown = true;
    }
    else if (key == kVK_Control)
    {
        ctrlDown = true;
    }
    else if (key == kVK_Option)
    {
        altDown = true;
    }

    CGEventSourceRef source = NULL;
    if (shiftDown || ctrlDown || altDown)
    {
        source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    }

    CGEventRef command = CGEventCreateKeyboardEvent(source, key, true);
    if (c) CGEventKeyboardSetUnicodeString(command, 1, &c);
    if (shiftDown) CGEventSetFlags(command, kCGEventFlagMaskShift);
    if (ctrlDown) CGEventSetFlags(command, kCGEventFlagMaskControl);
    if (altDown) CGEventSetFlags(command, kCGEventFlagMaskAlternate);
    CGEventPost(kCGAnnotatedSessionEventTap, command);
    CFRelease(command);
}

void KeyboardMac::sendKeyUp(CGKeyCode key, UniChar c)
{
    if (key == kVK_Shift)
    {
        shiftDown = false;
    }
    else if (key == kVK_Control)
    {
        ctrlDown = false;
    }
    else if (key == kVK_Option)
    {
        altDown = false;
    }
    CGEventRef command = CGEventCreateKeyboardEvent(NULL, key, false);
    if (c) CGEventKeyboardSetUnicodeString(command, 1, &c);
    CGEventPost(kCGAnnotatedSessionEventTap, command);
    CFRelease(command);
}

void KeyboardMac::sendEachKey(QString message)
{
    for (int i = 0; i < message.length(); ++i)
    {
        QChar character = message.at(i);
        sendUnicodeKeyPress(character);
    }
}

void KeyboardMac::sendShortcut(CGKeyCode keyCode, CGEventFlags flags)
{
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventRef commandDown = CGEventCreateKeyboardEvent(source, keyCode, true);
    CGEventSetFlags(commandDown, flags);
    CGEventRef commandUp = CGEventCreateKeyboardEvent(source, keyCode, false);

    CGEventPost(kCGAnnotatedSessionEventTap, commandDown);
    CGEventPost(kCGAnnotatedSessionEventTap, commandUp);

    CFRelease(commandUp);
    CFRelease(commandDown);
    CFRelease(source);
}

void KeyboardMac::keycodeToShortcut(int keyCode)
{
    // todo
    switch (keyCode)
    {
    case KEYCODE_COPY:
        sendShortcut(kVK_ANSI_C, kCGEventFlagMaskCommand);
        break;
    case KEYCODE_PASTE:
        sendShortcut(kVK_ANSI_V, kCGEventFlagMaskCommand);
        break;
    case KEYCODE_SELECT_ALL:
        sendShortcut(kVK_ANSI_A, kCGEventFlagMaskCommand);
        break;
    case KEYCODE_CUT:
        sendShortcut(kVK_ANSI_X, kCGEventFlagMaskCommand);
        break;
    default:
        Logger::Instance()->add("Unkown keyboard command");
        break;
    }
}

CGKeyCode KeyboardMac::keycodeToKey(int keyCode)
{
    switch (keyCode)
    {
    case KEYCODE_BACK:
        return kVK_Delete;
    case KEYCODE_CAPS_LOCK:
        shiftDown = (shiftDown) ? false : true;
        return -1;
    case KEYCODE_DEL:
        return kVK_Delete;
    case KEYCODE_ENTER:
        return kVK_Return;
    case KEYCODE_ESCAPE:
        return kVK_Escape;
    case KEYCODE_INSERT:
        return kVK_ForwardDelete;
    case KEYCODE_MOVE_END:
        return kVK_End;
    case KEYCODE_MOVE_HOME:
        return kVK_Home;
    case KEYCODE_PAGE_DOWN:
        return kVK_PageDown;
    case KEYCODE_PAGE_UP:
        return kVK_PageUp;
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
    case KEYCODE_WINDOWS:
    {
        // no windows key on a mac
        return -1;
    }
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
        return -1;
    }
}



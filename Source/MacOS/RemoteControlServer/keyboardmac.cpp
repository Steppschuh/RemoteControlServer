#include "keyboardmac.h"
#include "logger.h"

#include <QObject>

#include <Carbon/Carbon.h>
#include <CoreServices/CoreServices.h>

#ifdef Q_OS_MAC
    #include <ApplicationServices/ApplicationServices.h>
    #include "macx.h"
#endif

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
    if (key == kVK_Shift) shiftDown = true;
    else if (key == kVK_Function) fnDown = true;
    else if (key == kVK_Control) ctrlDown = true;
    else if (key == kVK_Option) altDown = true;
    else if (key == kVK_Command) cmdDown = true;

    CGEventSourceRef source = NULL;
    if (shiftDown || ctrlDown || altDown || cmdDown)
    {
        source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    }

    CGEventRef command = CGEventCreateKeyboardEvent(source, key, true);
    CGEventFlags flags;
    bool flagsAreInitialized = false;
    if (c) CGEventKeyboardSetUnicodeString(command, 1, &c);
    if (shiftDown)
    {
        // as this is the first if condition, it is impossible that there is already a flag set
        flags = kCGEventFlagMaskShift;
        flagsAreInitialized = true;
    }
    if (fnDown)
    {
        if (flagsAreInitialized) flags = CGEventFlags (flags | kCGEventFlagMaskSecondaryFn);
        else flags = kCGEventFlagMaskSecondaryFn;
        flagsAreInitialized = true;
    }
    if (ctrlDown)
    {
        if (flagsAreInitialized) flags = CGEventFlags (flags | kCGEventFlagMaskControl);
        else flags = kCGEventFlagMaskControl;
        flagsAreInitialized = true;
    }
    if (altDown)
    {
        if (flagsAreInitialized) flags = CGEventFlags (flags | kCGEventFlagMaskAlternate);
        else flags = kCGEventFlagMaskAlternate;
        flagsAreInitialized = true;
    }
    if (cmdDown)
    {
        if (flagsAreInitialized) flags = CGEventFlags (flags | kCGEventFlagMaskCommand);
        else flags = kCGEventFlagMaskCommand;
        flagsAreInitialized = true;
    }
    if (flagsAreInitialized) CGEventSetFlags(command, flags);
    CGEventPost(kCGAnnotatedSessionEventTap, command);
    CFRelease(command);
}

void KeyboardMac::sendKeyUp(CGKeyCode key, UniChar c)
{
    if (key == kVK_Shift) shiftDown = false;
    else if (key == kVK_Function) fnDown = false;
    else if (key == kVK_Control) ctrlDown = false;
    else if (key == kVK_Option) altDown = false;
    else if (key == kVK_Command) cmdDown = false;

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

void KeyboardMac::sendShortcut(int keyCode)
{
    switch (keyCode)
    {
    case KEYCODE_COPY:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_C);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_PASTE:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_V);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_SELECT_ALL:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_A);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_CUT:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_X);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_SHOW_DESKTOP:
    {
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(keycodeToKey(KEYCODE_F3));
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    }
    case KEYCODE_ZOOM_IN:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_KeypadPlus);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_ZOOM_OUT:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_KeypadMinus);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_CLOSE:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_Q);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_SHUTDOWN:
        shutdown();
        break;
    case KEYCODE_STANDBY:
        standby();
        break;
    case KEYCODE_CANCEL:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_Period);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_REFRESH:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_R);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_FULLSCREEN:
        sendKeyDown(keycodeToKey(KEYCODE_CONTROL));
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_F);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        sendKeyUp(keycodeToKey(KEYCODE_CONTROL));
        break;
    case KEYCODE_UNDO:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_ANSI_Z);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_BROWSER_BACK:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_LeftArrow);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    case KEYCODE_BROWSER_FORWARD:
        sendKeyDown(keycodeToKey(KEYCODE_COMMAND));
        sendKeyPress(kVK_RightArrow);
        sendKeyUp(keycodeToKey(KEYCODE_COMMAND));
        break;
    default:
        //Logger::Instance()->add("Unkown keyboard command");
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
        return KEYCODE_UNKOWN;
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
    case KEYCODE_COMMAND:
        return kVK_Command;
    case KEYCODE_FUNCTION:
        return kVK_Function;
    case KEYCODE_SHIFT:
        return kVK_Shift;
    case KEYCODE_DEL_FORWARD:
        return kVK_ForwardDelete;
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
        return KEYCODE_UNKOWN;
    }
}

void KeyboardMac::standby()
{
    MDSendAppleEventToSystemProcess(kAESleep);
}

void KeyboardMac::shutdown()
{
    MDSendAppleEventToSystemProcess(kAEShutDown);
}



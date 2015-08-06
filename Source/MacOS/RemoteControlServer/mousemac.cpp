#include "mousemac.h"
#include "mousev3.h"

#include <stdlib.h>

#ifdef Q_OS_MAC
    #include <ApplicationServices/ApplicationServices.h>
#endif

#include <QDebug>

MouseMac *MouseMac::instance = NULL;

MouseMac *MouseMac::Instance()
{
    if (!instance)
    {
        instance = new MouseMac();
    }
    return instance;
}

MouseMac::MouseMac()
{

}

CGPoint MouseMac::getCursorPositionCGPoint()
{
    CGEventRef event = CGEventCreate(NULL);
    return CGEventGetLocation(event);
}

QPoint *MouseMac::getCursorPosition()
{
    CGPoint point = getCursorPositionCGPoint();
    return new QPoint(point.x, point.y);
}

void MouseMac::leftMouseDown(bool isDoubleClick)
{
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventLeftMouseDown,
                getCursorPositionCGPoint(),
                kCGMouseButtonLeft);
    if (isDoubleClick)
    {
        CGEventSetIntegerValueField(mouseEv, kCGMouseEventClickState, 2);
    }
    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}

void MouseMac::leftMouseUp(bool isDoubleClick)
{
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventLeftMouseUp,
                getCursorPositionCGPoint(),
                kCGMouseButtonLeft);
    if (isDoubleClick)
    {
        CGEventSetIntegerValueField(mouseEv, kCGMouseEventClickState, 2);
    }
    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}

bool MouseMac::moveMouseTo(int x, int y)
{
    UInt32 maxDisplays = 4;
    CGDirectDisplayID displayID[maxDisplays];
    CGDisplayCount c = 0;
    CGDisplayCount *count = &c;
    CGGetDisplaysWithPoint(
                CGPointMake(x, y),
                maxDisplays,
                displayID,
                count);
    if (*count > 0)
    {
        CGEventRef mouseEv = CGEventCreateMouseEvent(
                        NULL, kCGEventMouseMoved,
                        CGPointMake(x, y),
                        kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, mouseEv);
        CFRelease(mouseEv);
        return true;
    }
    else
    {
        return false;
    }
}

void MouseMac::mouseScrollVertical(int scrollDirection, int scrollLength)
{
    if (scrollDirection < -10 || scrollDirection == 0 || scrollDirection > 10)
    {
        scrollDirection = 1;
    }
    scrollDirection = scrollDirection * scrollLength / 100;
    scrollDirection = (scrollDirection < -10) ? -10 : ((scrollDirection > 10) ? 10 : scrollDirection);
    CGEventRef scrollEvent = CGEventCreateScrollWheelEvent (
                        NULL, kCGScrollEventUnitLine,  // kCGScrollEventUnitLine  //kCGScrollEventUnitPixel
                        1, //CGWheelCount 1 = y 2 = xy 3 = xyz
                        scrollDirection); // length of scroll from -10 to 10 higher values lead to undef behaviour

    CGEventPost(kCGHIDEventTap, scrollEvent);

    CFRelease(scrollEvent);
}

void MouseMac::rightMouseDown()
{
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventRightMouseDown,
                getCursorPositionCGPoint(),
                kCGMouseButtonRight);
    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}

void MouseMac::rightMouseUp()
{
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventRightMouseUp,
                getCursorPositionCGPoint(),
                kCGMouseButtonRight);
    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}


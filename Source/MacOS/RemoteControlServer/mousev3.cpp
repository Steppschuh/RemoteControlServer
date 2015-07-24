#include "logger.h"
#include "mousev3.h"
#include "server.h"
#include "settings.h"
#include "touchpoint.h"

#include <QDateTime>
#include <QList>

#include "math.h"

#include <QDebug>

MouseV3* MouseV3::instance = NULL;

MouseV3* MouseV3::Instance()
{
    if (!instance)
    {
        instance = new MouseV3();
    }
    return instance;
}

MouseV3::MouseV3()
{
    cursorPositionNew = new QPoint();
    P_ORIGIN = new QPoint();
    P1_New = new QPoint;
    P1_Rel = new QPoint;
    P1_Start = new QPoint;
    P2_Start = new QPoint;
    P3_Start = new QPoint;
    P1_Last = new QPoint;
    P2_Last = new QPoint;
    P3_Last = new QPoint;
    downEventIndex = -1;
    moveEventIndex = -1;
}

void MouseV3::leftMouseDown()
{
    mouseLeftDown = true;
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventLeftMouseDown,
                getCursorPosition(),
                kCGMouseButtonLeft);

    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}

void MouseV3::leftMouseUp()
{
    mouseLeftDown = false;
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventLeftMouseUp,
                getCursorPosition(),
                kCGMouseButtonLeft);

    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}

void MouseV3::rightMouseDown()
{
    mouseRightDown = true;
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventRightMouseDown,
                getCursorPosition(),
                kCGMouseButtonRight);

    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}

void MouseV3::rightMouseUp()
{
    mouseRightDown = false;
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventRightMouseUp,
                getCursorPosition(),
                kCGMouseButtonRight);

    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}

void MouseV3::pointersDown()
{
    if (mousePadDown)
    {
        return;
    }

    mousePadDown = true;
    P1_Start = P_ORIGIN;
    P1_Last = P_ORIGIN;

    pointers = new QList<TouchPoint*>();
    TouchPoint* currentPointer = new TouchPoint();
    currentPointer->x = 0;
    currentPointer->y = 0;
    pointers->append(currentPointer);

    pointDown = new TouchPoint();
    pointDown->timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void MouseV3::pointersUp()
{
    checkForClick();

    P1_Start = P_ORIGIN;
    P1_Last = P_ORIGIN;
    P2_Start = P_ORIGIN;
    P2_Last = P_ORIGIN;
    P3_Start = P_ORIGIN;
    P3_Last = P_ORIGIN;
    P3_Vector_Start = 0;
    P3_Vector_Last = 0;

    mousePadDown = false;
    // currentGesture = GESTURE_NONE
    pointers = new QList<TouchPoint*>();
}

void MouseV3::parsePointerData(QByteArray &data)
{
    if (data.length() >= 7)
    {
        unsigned char firstByte = data.at(3);
        unsigned char secondByte = data.at(4);
        int newDownEventIndex = firstByte << 8 | secondByte;
        firstByte = data.at(5);
        secondByte = data.at(6);
        int newMoveEventIndex = firstByte << 8 | secondByte;

        int pointerDataOffset = 7;
        int pointerLength = 4;
        int pointerCount = (data.length() - pointerDataOffset) / pointerLength;

        pointers = new QList<TouchPoint*>();

        for (int i = 0; i < pointerCount; ++i)
        {
            TouchPoint *currentPointer = new TouchPoint();
            firstByte = data.at(pointerDataOffset + (i * pointerLength));
            secondByte = data.at(pointerDataOffset + (i * pointerLength) + 1);
            int x = firstByte << 8 | secondByte;

            firstByte = data.at(pointerDataOffset + (i * pointerLength) + 2);
            secondByte = data.at(pointerDataOffset + (i * pointerLength) + 3);
            int y = firstByte << 8 | secondByte;

            currentPointer->x = x;
            currentPointer->y = y;

            pointers->append(currentPointer);
        }

        if (newDownEventIndex > downEventIndex)
        {
            moveEventIndex = -1;
            pointersUp();
        }

        downEventIndex = newDownEventIndex;
        moveEventIndex = newMoveEventIndex;

        if (pointers->length() == 1)
        {
            updateCursorPosition();
        }
        else if (pointers->length() > 1)
        {
            parseMultitouch();
        }
    }
    else
    {
        qDebug() << "Not enough data for parsePointerData";
    }
}

void MouseV3::parseAbsolutePointerData(QByteArray &data, bool isPresenter = false)
{
    int absoluteMaximum = 32767;
    int pointerDataOffset = 3;

    pointers = new QList<TouchPoint*>();

    TouchPoint *currentPointer = new TouchPoint();

    int x = data.at(pointerDataOffset);
    x = (x << 8) + data.at(pointerDataOffset + 1);

    int y = data.at(pointerDataOffset + 2);
    y = (y << 8) + data.at(pointerDataOffset + 3);

    QList<QPoint*> *locations = Screenshot::Instance()->getScreenBounds(Screenshot::Instance()->screenIndex);
    QPoint* startLocation = locations->at(0);
    QPoint* endLocation = locations->at(1);
    Size screenSize = {endLocation->x() - startLocation->x(), endLocation->y() - startLocation->y()};

    currentPointer->x = (x * screenSize.Width) / absoluteMaximum;
    currentPointer->y = (y * screenSize.Height) / absoluteMaximum;

    cursorPositionNew->setX(startLocation->x() + currentPointer->x);
    cursorPositionNew->setY(startLocation->y() + currentPointer->y);

    pointers->append(currentPointer);

    P1_Start->setX(currentPointer->x);
    P1_Start->setY(currentPointer->y);

    moveMouseTo(cursorPositionNew->x(), cursorPositionNew->y());

    if (isPresenter)
    {
        updatePointerPosition();
    }
}

int MouseV3::trim(int value, int min, int max)
{
    return (value < min) ? min : ((value > max) ? max : value);
}

void MouseV3::moveMouseTo(int x, int y)
{
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                    NULL, kCGEventMouseMoved,
                    CGPointMake(x, y),
                    kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, mouseEv);
    CFRelease(mouseEv);
}

void MouseV3::updatePointerPosition()
{
    if (pointers->length() > 0)
    {
//        Server::Instance()->pointer.showPointer
        Logger::Instance()->add("Showing pointer");
        // todo
    }
}

void MouseV3::updateCursorPosition()
{
    if (!mousePadDown)
    {
        pointersDown();
        return;
    }
    P1_New = new QPoint(pointers->at(0)->x, pointers->at(0)->y);

    if (P1_Start == P_ORIGIN || P1_Last == P_ORIGIN)
    {
        P1_Start = P1_New;
        P1_Last = P1_New;
        return;
    }

    cursorPositionCurrent = getCursorPosition();

    if (Settings::Instance()->mouseAcceleration == 1.0)
    {
        P1_Rel->setX((P1_New->x() - P1_Last->x()) * Settings::Instance()->mouseSensitivity);
        P1_Rel->setY((P1_New->y() - P1_Last->y()) * Settings::Instance()->mouseSensitivity);
    }
    else
    {
        P1_Rel->setX(P1_New->x() - P1_Last->x());
        P1_Rel->setY(P1_New->y() - P1_Last->x());

        if (P1_Rel->x() != 0 || P1_Rel->y() != 0)
        {
            if (P1_Rel->x() > 0)
            {
                P1_Rel->setX(pow(P1_Rel->x(), Settings::Instance()->mouseAcceleration) * Settings::Instance()->mouseSensitivity);
            }
            else
            {
                P1_Rel->setX(P1_Rel->x() * -1);
                P1_Rel->setX(pow(P1_Rel->x(), Settings::Instance()->mouseAcceleration));
                P1_Rel->setX(P1_Rel->x() * Settings::Instance()->mouseSensitivity);
                P1_Rel->setX(P1_Rel->x() * -1);
            }

            if (P1_Rel->y() > 0)
            {
                P1_Rel->setY(pow(P1_Rel->y(), Settings::Instance()->mouseAcceleration) * Settings::Instance()->mouseSensitivity);
            }
            else
            {
                P1_Rel->setY(P1_Rel->y() * -1);
                P1_Rel->setY(pow(P1_Rel->y(), Settings::Instance()->mouseAcceleration));
                P1_Rel->setY(P1_Rel->y() * Settings::Instance()->mouseSensitivity);
                P1_Rel->setY(P1_Rel->y() * -1);
            }
        }
    }

    if (P1_Rel->x() < -700 || P1_Rel->x() > 700 || P1_Rel->y() < -700 || P1_Rel->y() > 700)
    {
        pointersUp();
        return;
    }

    P1_Last = P1_New;

    CGDirectDisplayID displayID = CGMainDisplayID();
    int width = CGDisplayPixelsWide(displayID);
    int height = CGDisplayPixelsHigh(displayID);

    cursorPositionNew->setX(trim(cursorPositionCurrent.x + P1_Rel->x(), 0, width));
    cursorPositionNew->setY(trim(cursorPositionCurrent.y + P1_Rel->y(), 0, height));
    moveMouseTo(cursorPositionNew->x(), cursorPositionNew->y());
}

void MouseV3::parseMultitouch()
{
    if (!mousePadDown)
    {
        pointersDown();
        return;
    }
}

void MouseV3::checkForClick()
{
    //todo
}

CGPoint MouseV3::getCursorPosition()
{
    CGEventRef event = CGEventCreate(NULL);
    return CGEventGetLocation(event);
}

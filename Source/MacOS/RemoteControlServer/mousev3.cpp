#include "converter.h"
#include "logger.h"
#include "mousev3.h"
#include "screenshot.h"
#include "server.h"
#include "settings.h"
#include "touchpoint.h"

#ifdef Q_OS_MAC
    #include "keyboardmac.h"
    #include "mousemac.h"
#endif

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
    P_ORIGIN = new QPoint(0, 0);
    P1_New = new QPoint();
    P2_New = new QPoint();
    P3_New = new QPoint();
    P1_Rel = new QPoint();
    P1_Start = new QPoint();
    P2_Start = new QPoint();
    P3_Start = new QPoint();
    P1_Last = new QPoint();
    P2_Last = new QPoint();
    P3_Last = new QPoint();
    downEventIndex = -1;
    moveEventIndex = -1;

    pointDown = new TouchPoint();
}

void MouseV3::leftSecondClick()
{
    MouseMac::Instance()->leftMouseDown(true);
    MouseMac::Instance()->leftMouseUp(true);
}

void MouseV3::pointersDown()
{
    if (mousePadDown)
    {
        return;
    }

    mousePadDown = true;
    P1_Start = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
    P1_Last = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());

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
    P1_Start = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
    P1_Last = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
    P2_Start = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
    P2_Last = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
    P3_Start = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
    P3_Last = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
    P3_Vector_Start = 0;
    P3_Vector_Last = 0;

    mousePadDown = false;
    currentGesture = GESTURE_NONE;
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

    unsigned char firstByte = data.at(pointerDataOffset);
    unsigned char secondByte = data.at(pointerDataOffset + 1);
    int x = firstByte << 8 | secondByte;

    firstByte = data.at(pointerDataOffset + 2);
    secondByte = data.at(pointerDataOffset + 3);
    int y = firstByte << 8 | secondByte;

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

    MouseMac::Instance()->moveMouseTo(cursorPositionNew->x(), cursorPositionNew->y());

    if (isPresenter)
    {
        updatePointerPosition();
    }
}

int MouseV3::trim(int value, int min, int max)
{
    return (value < min) ? min : ((value > max) ? max : value);
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

    if ((P1_Start->x() == P_ORIGIN->x() && P1_Start->y() == P_ORIGIN->y()) || (P1_Last->x() == P_ORIGIN->x() && P1_Last->y() == P_ORIGIN->y()))
    {
        P1_Start = new QPoint(P1_New->x(), P1_New->y());
        P1_Last = new QPoint(P1_New->x(), P1_New->y());
        return;
    }

    cursorPositionCurrent = MouseMac::Instance()->getCursorPosition();

    if (Settings::Instance()->mouseAcceleration == 1.0)
    {
        P1_Rel->setX((P1_New->x() - P1_Last->x()) * Settings::Instance()->mouseSensitivity);
        P1_Rel->setY((P1_New->y() - P1_Last->y()) * Settings::Instance()->mouseSensitivity);
    }
    else
    {
        P1_Rel->setX(P1_New->x() - P1_Last->x());
        P1_Rel->setY(P1_New->y() - P1_Last->y());

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

    P1_Last = new QPoint(P1_New->x(), P1_New->y());

    int newX = cursorPositionCurrent->x() + P1_Rel->x();
    int newY = cursorPositionCurrent->y() + P1_Rel->y();
    if (MouseMac::Instance()->moveMouseTo(newX, newY))
    {
        cursorPositionNew->setX(newX);
        cursorPositionNew->setY(newY);
    }
}

void MouseV3::parseMultitouch()
{
    if (!mousePadDown)
    {
        pointersDown();
        return;
    }

    P1_New = new QPoint(pointers->at(0)->x, pointers->at(0)->y);
    P2_New = new QPoint(pointers->at(1)->x, pointers->at(1)->y);

    if ((P1_Start->x() == P_ORIGIN->x() && P1_Start->y() == P_ORIGIN->y())
            || (P1_Last->x() == P_ORIGIN->x() && P1_Last->y() == P_ORIGIN->y())
            || (P2_Start->x() == P_ORIGIN->x() && P2_Start->y() == P_ORIGIN->y())
            || (P2_Last->x() == P_ORIGIN->x() && P2_Last->y() == P_ORIGIN->y()))
    {

        P1_Start = new QPoint(P1_New->x(), P1_New->y());
        P1_Last = new QPoint(P1_New->x(), P1_New->y());
        P2_Start = new QPoint(P2_New->x(), P2_New->y());
        P2_Last = new QPoint(P2_New->x(), P2_New->y());
        return;
    }

    P1_Rel->setX((P1_New->x() - P1_Last->x()) * Settings::Instance()->mouseSensitivity);
    P1_Rel->setY((P1_New->y() - P1_Last->y()) * Settings::Instance()->mouseSensitivity);

    if (P1_Rel->x() < -700 || P1_Rel->x() > 700 || P1_Rel->y() < -700 || P1_Rel->y() > 700)
    {
        pointersUp();
        return;
    }

    P1_Last = new QPoint(P1_New->x(), P1_New->y());
    P2_Last = new QPoint(P1_New->x(), P1_New->y());

    processMultitouch();
}

void MouseV3::processMultitouch()
{
    P3_New->setX(round((P1_New->x() + P2_New->x()) / 2.0));
    P3_New->setY(round((P1_New->y() + P2_New->y()) / 2.0));
    P3_Vector_New = Converter::Instance()->getPointDistance(*P1_New, *P2_New);

    if (P3_Start->x() == P_ORIGIN->x() && P3_Start->y() == P_ORIGIN->y())
    {
        P3_Start = new QPoint(P3_New->x(), P3_New->y());
    }

    if (P3_Vector_Start == 0)
    {
        P3_Vector_Start = P3_Vector_New;
    }

    if (currentGesture == GESTURE_NONE)
    {
        if (valueMatchesTolerance(P3_Vector_New, P3_Vector_Start, SCROLL_OFFSET_TOLERANCE))
        {
            if (valueMatchesTolerance(P3_New->x(), P3_Start->x(), SCROLL_OFFSET_TOLERANCE))
            {
                if (!valueMatchesTolerance(P3_New->y(), P3_Start->y(), SCROLL_OFFSET_TOLERANCE / 2))
                {
                    currentGesture = GESTURE_SCROLL;
                }
            }
        }
        else
        {
            currentGesture = GESTURE_ZOOM;
            P3_Vector_Event = P3_Vector_New + 20;
        }
    }

    if (currentGesture == GESTURE_ZOOM)
    {
        if (!valueMatchesTolerance(P3_Vector_New, P3_Vector_Event, 25))
        {
            scrollAmount = pow((P3_Vector_New = P3_Vector_Last), 2);
            if (P3_Vector_New > P3_Vector_Last)
            {
                MouseMac::Instance()->zoom(1, 1);
            }
            else
            {
                MouseMac::Instance()->zoom(-1, 1);
            }
            P3_Vector_Event = P3_Vector_New;
        }
    }
    else if (currentGesture == GESTURE_SCROLL)
    {
        scrollAmount = abs(pow(P3_New->y() - P3_Last->y(), 2));
        if (P3_New->y() > P3_Last->y())
        {
            MouseMac::Instance()->mouseScrollVertical(1, scrollAmount);
        }
        else if (P3_New->y() < P3_Last->y())
        {
            MouseMac::Instance()->mouseScrollVertical(-1, scrollAmount);
        }
    }

    P3_Last = new QPoint(P3_New->x(), P3_New->y());
    P3_Vector_Last = P3_Vector_New;
}

void MouseV3::checkForClick()
{
    long timeDelta = QDateTime::currentDateTime().toMSecsSinceEpoch() - pointDown->timestamp;

    if (timeDelta < CLICK_DELAY_TOLERANCE)
    {
        if (pointers->length() >= 1)
        {
            Logger::Instance()->add("CLick delay " + timeDelta);
            int offsetX = abs(P1_Start->x() - pointers->at(0)->x);
            int offsetY = abs(P1_Start->y() - pointers->at(0)->y);

            if (offsetX < CLICK_OFFSET_TOLERANCE && offsetY < CLICK_OFFSET_TOLERANCE)
            {
                leftSecondClick();
            }
        }
    }
}

bool MouseV3::valueMatchesTolerance(float val1, float val2, int tolerance)
{
    return (val1 < (val2 + tolerance) && val1 > (val2 - tolerance)) ? true : false;
}

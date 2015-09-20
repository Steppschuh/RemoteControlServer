#include "converter.h"
#include "mousev2.h"
#include "screenshot.h"
#include "server.h"
#include "settings.h"

#include <math.h>

#ifdef Q_OS_MAC
    #include "keyboardmac.h"
    #include "mousemac.h"
#endif

#include <QDateTime>
#include <QDesktopWidget>

#include <QDebug>

MouseV2 *MouseV2::instance = NULL;

MouseV2 *MouseV2::Instance()
{
    if (!instance)
    {
        instance = new MouseV2();
    }
    return instance;
}

MouseV2::MouseV2()
{
    currentGesture = 0;

    cursorPositionNew = new QPoint();
    cursorPositionCurrent = new QPoint();
    cursorPositionDown = new QPoint();
    P_ORIGIN = new QPoint(0, 0);
    P1_New = new QPoint();
    P2_New = new QPoint();
    P3_New = new QPoint();
    P1_Rel = new QPoint();
    P2_Rel = new QPoint();
    P1_Start = new QPoint();
    P2_Start = new QPoint();
    P3_Start = new QPoint();
    P1_Last = new QPoint();
    P2_Last = new QPoint();
    P3_Last = new QPoint();

    P1_Down = QDateTime::currentDateTime().toMSecsSinceEpoch();
    P1_Up = QDateTime::currentDateTime().toMSecsSinceEpoch();
    P2_Down = QDateTime::currentDateTime().toMSecsSinceEpoch();
    P2_Up = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

bool MouseV2::valueMatchesTolerance(float val1, float val2, int tolerance)
{
    return (val1 < (val2 + tolerance) && val1 > (val2 - tolerance)) ? true : false;
}

QPoint *MouseV2::commandGetPoint(QByteArray &messageBytes, int ID)
{
    QPoint *p = new QPoint();
    if (ID == 0 && messageBytes.length() >= 5)
    {
        unsigned char firstByte = messageBytes.at(3);
        unsigned char secondByte = messageBytes.at(4);
        p->setX(firstByte);
        p->setY(secondByte);
    }
    else if (ID == 1 && messageBytes.length() >= 7)
    {
        unsigned char firstByte = messageBytes.at(5);
        unsigned char secondByte = messageBytes.at(6);
        p->setX(firstByte);
        p->setY(secondByte);
    }
    return p;
}

bool MouseV2::isPointerDown(QPoint &point)
{
    return (point.x() == P_ORIGIN->x() && point.y() == P_ORIGIN->y()) ? false : true;
}

void MouseV2::moveCursor()
{
#ifdef Q_OS_MAC
    cursorPositionCurrent = MouseMac::Instance()->getCursorPosition();
#endif
    if (isPointerDown(*P2_New))
    {
        // 2 Pointer down
        if (mouseLeftDown)
        {
            // Left mouse down and moving
            P2_Rel->setX((P2_New->x() - P2_Last->x()) * Settings::Instance()->mouseSensitivity);
            P2_Rel->setY((P2_New->y() - P2_Last->y()) * Settings::Instance()->mouseSensitivity);
            cursorPositionNew = new QPoint(cursorPositionCurrent->x() + P2_Rel->x(), cursorPositionCurrent->y() + P2_Rel->y());
            P2_Last = new QPoint(P2_New->x(), P2_New->y());
#ifdef Q_OS_MAC
            MouseMac::Instance()->moveMouseTo(cursorPositionNew->x(), cursorPositionNew->y());
#endif
        }
    }
    else
    {
        // 1 Pointer down
        if (P1_Last->x() == P_ORIGIN->x() && P1_Last->y() == P_ORIGIN->y())
        {
            // Start of mouse move
            P1_Last = new QPoint(P1_New->x(), P1_New->y());
        }
        else
        {
            if (Settings::Instance()->mouseAcceleration == 1)
            {
                // No acceleration
                P1_Rel->setX((P1_New->x() - P1_Last->x()) * Settings::Instance()->mouseSensitivity);
                P1_Rel->setY((P1_New->y() - P1_Last->y()) * Settings::Instance()->mouseSensitivity);
            }
            else
            {
                // Acceleration
                P1_Rel->setX((P1_New->x() - P1_Last->x()));
                P1_Rel->setY((P1_New->y() - P1_Last->y()));

                if (P1_Rel->x() > 0)
                {
                    P1_Rel->setX(pow(P1_Rel->x(), Settings::Instance()->mouseAcceleration) * Settings::Instance()->mouseSensitivity);
                }
                else
                {
                    P1_Rel->setX(P1_Rel->x() * -1);
                    P1_Rel->setX(pow(P1_Rel->x(), Settings::Instance()->mouseAcceleration) * Settings::Instance()->mouseSensitivity);
                    P1_Rel->setX(P1_Rel->x() * -1);
                }

                if (P1_Rel->y() > 0)
                {
                    P1_Rel->setY(pow(P1_Rel->y(), Settings::Instance()->mouseAcceleration) * Settings::Instance()->mouseSensitivity);
                }
                else
                {
                    P1_Rel->setY(P1_Rel->y() * -1);
                    P1_Rel->setY(pow(P1_Rel->y(), Settings::Instance()->mouseAcceleration) * Settings::Instance()->mouseSensitivity);
                    P1_Rel->setY(P1_Rel->y() * -1);
                }
            }

            cursorPositionNew = new QPoint(cursorPositionCurrent->x() + P1_Rel->x(), cursorPositionCurrent->y() + P1_Rel->y());
            P1_Last = new QPoint(P1_New->x(), P1_New->y());
#ifdef Q_OS_MAC
            MouseMac::Instance()->moveMouseTo(cursorPositionNew->x(), cursorPositionNew->y());
#endif
        }
    }
}

void MouseV2::leftClickRepeat(int count)
{
    for (int i = 0; i < count; ++i)
    {
#ifdef Q_OS_MAC
        MouseMac::Instance()->leftMouseDown(false);
        MouseMac::Instance()->leftMouseUp(false);
#endif
    }
}

void MouseV2::processMultitouch()
{
    P3_New->setX(round((P1_New->x() + P2_New->x()) / 2.0));
    P3_New->setY(round((P1_New->y() + P2_New->y()) / 2.0));
    P3_Vector_New = Converter::Instance()->getPointDistance(*P1_New, *P2_New);

    if (currentGesture == GESTURE_NONE)
    {
        if (valueMatchesTolerance(P3_Vector_New, P3_Vector_Start, 40))
        {
            if (valueMatchesTolerance(P3_New->x(), P3_Start->x(), 40) && !valueMatchesTolerance(P3_New->y(), P3_Start->y(), 40))
            {
                currentGesture = GESTURE_SCROLL;
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
            scrollAmount = pow(P3_Vector_New - P3_Vector_Last, 2);
#ifdef Q_OS_MAC
            if (P3_Vector_New > P3_Vector_Last) MouseMac::Instance()->zoom(1, 1);
            else MouseMac::Instance()->zoom(-1, 1);
#endif
            P3_Vector_Event = P3_Vector_New;
        }
    }
    else if (currentGesture == GESTURE_SCROLL)
    {
        scrollAmount = pow(P3_New->y() - P3_Last->y(), 2);
#ifdef Q_OS_MAC
        if (P3_New->y() > P3_Last->y()) MouseMac::Instance()->mouseScrollVertical(1, scrollAmount);
        else if (P3_New->y() < P3_Last->y()) MouseMac::Instance()->mouseScrollVertical(-1, scrollAmount);
#endif
    }

    P3_Last = new QPoint(P3_New->x(), P3_New->y());
    P3_Vector_Last = P3_Vector_New;
}

void MouseV2::parseCursorMove(QByteArray &messageBytes)
{
    P1_New = commandGetPoint(messageBytes, 0);
    if (isMultitouch)
    {
        P2_New = commandGetPoint(messageBytes, 1);
        processMultitouch();
    }
    else
    {
        P2_New = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
        moveCursor();
    }
}

void MouseV2::parseCursorSet(QByteArray &messageBytes)
{
    int x = 0;
    int y = 0;
    if (messageBytes.length() >= 7)
    {
        unsigned char firstByte = messageBytes.at(3);
        unsigned char secondByte = messageBytes.at(4);
        x = firstByte << 8 | secondByte;
        firstByte = messageBytes.at(5);
        secondByte = messageBytes.at(6);
        y = firstByte << 8 | secondByte;
#ifdef Q_OS_MAC
        MouseMac::Instance()->moveMouseTo(x, y);
#endif
    }
}

void MouseV2::parseScroll(QByteArray &messageBytes)
{
    P1_New = commandGetPoint(messageBytes, 0);
    scrollAmount = abs((P1_New->y() - P1_Last->y()) * 20);
#ifdef Q_OS_MAC
    if (P1_New->y() > P1_Last->y()) MouseMac::Instance()->mouseScrollVertical(1, scrollAmount);
    else if (P1_New->y() < P1_Last->y()) MouseMac::Instance()->mouseScrollVertical(-1, scrollAmount);
#endif
    P1_Last = new QPoint(P1_New->x(), P1_New->y());
}

void MouseV2::parseClick(QByteArray &messageBytes)
{
    if (messageBytes.length() >= 3)
    {
        switch (messageBytes.at(2))
        {
        case cmd_mouse_down:
            isMultitouch = false;
            P1_Start = commandGetPoint(messageBytes, 0);
            P1_Last = new QPoint(P1_Start->x(), P1_Start->y());
#ifdef Q_OS_MAC
            cursorPositionDown = MouseMac::Instance()->getCursorPosition();
#endif
            mousePadDown = true;
            P1_Down = QDateTime::currentDateTime().toMSecsSinceEpoch();
            break;
        case cmd_mouse_up:
            mousePadDown = false;
            isMultitouch = false;

            P1_Up = QDateTime::currentDateTime().toMSecsSinceEpoch();

            if ((P1_Up - P1_Down < 150)
                    && (-10 < (P1_Start->x() - P1_Last->x())) && ((P1_Start->x() - P1_Last->x()) < 10)
                    && (-10 < (P1_Start->y() - P1_Last->y())) && ((P1_Start->y() - P1_Last->y()) < 10))
            {
                leftClickRepeat(1);
            }

            P1_Last = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
            P2_Last = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
            P1_New = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());

            break;
        case cmd_mouse_down_1:
            isMultitouch = true;
            P1_Start = commandGetPoint(messageBytes, 0);
            P1_New = new QPoint(P1_Start->x(), P1_Start->y());
            P1_Last = new QPoint(P1_Start->x(), P1_Start->y());
            P1_Down = QDateTime::currentDateTime().toMSecsSinceEpoch();

            P2_Start = commandGetPoint(messageBytes, 1);
            P2_New = new QPoint(P2_Start->x(), P2_Start->y());
            P2_Last = new QPoint(P2_Start->x(), P2_Start->y());
            P2_Down = QDateTime::currentDateTime().toMSecsSinceEpoch();
            currentGesture = GESTURE_NONE;

            P3_Start->setX(round((P1_New->x() + P2_New->x()) / 2.0));
            P3_Start->setY(round((P1_New->y() + P2_New->y()) / 2.0));
            P3_New = new QPoint(P3_Start->x(), P3_Start->y());
            P3_Last = new QPoint(P3_Start->x(), P3_Start->y());
            P3_Vector_Start = Converter::Instance()->getPointDistance(*P1_New, *P2_New);
            break;
        case cmd_mouse_up_1:
            P2_Up = QDateTime::currentDateTime().toMSecsSinceEpoch();
            isMultitouch = false;
            P1_Last = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
            P2_New = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
            P3_New = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
            P3_Last = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
            P3_Start = new QPoint(P_ORIGIN->x(), P_ORIGIN->y());
            break;
        case cmd_mouse_left_down:
#ifdef Q_OS_MAC
            MouseMac::Instance()->leftMouseDown(false);
#endif
            dateLeftDown = QDateTime::currentDateTime().toMSecsSinceEpoch();
            mouseLeftDown = true;
            break;
        case cmd_mouse_left_up:
#ifdef Q_OS_MAC
            MouseMac::Instance()->leftMouseUp(false);
#endif
            dateLeftUp = QDateTime::currentDateTime().toMSecsSinceEpoch();
            mouseLeftDown = false;
            break;
        case cmd_mouse_right_down:
#ifdef Q_OS_MAC
            MouseMac::Instance()->rightMouseDown();
#endif
            mouseRightDown = true;
            break;
        case cmd_mouse_right_up:
#ifdef Q_OS_MAC
            MouseMac::Instance()->rightMouseUp();
#endif
            mouseRightDown = false;
            break;
        case cmd_mouse_left_click:
#ifdef Q_OS_MAC
            MouseMac::Instance()->leftMouseDown(false);
            MouseMac::Instance()->leftMouseUp(false);
#endif
            break;
        case cmd_mouse_right_click:
#ifdef Q_OS_MAC
            MouseMac::Instance()->rightMouseDown();
            MouseMac::Instance()->rightMouseUp();
#endif
            break;
        }
    }
}

void MouseV2::parsePointer(QByteArray &messageBytes)
{
    if (messageBytes.length() >= 5)
    {
        unsigned char firstByte = messageBytes.at(3);
        unsigned char secondByte = messageBytes.at(4);
        X_New = firstByte;
        Y_New = secondByte;

        if (X_New > 99) X_New = 100 - X_New;
        if (Y_New > 99) Y_New = 100 - Y_New;

        X_New = X_New - X_Def;
        Y_New = Y_New - Y_Def;

        X_Rel = ((Y_New * -1) * Settings::Instance()->motionAcceleration * 20) / 100;
        Y_Rel = ((X_New * 1) * Settings::Instance()->motionAcceleration * 20) / 100;

        if (!(X_Rel > Settings::Instance()->motionFilter || X_Rel < Settings::Instance()->motionFilter * -1))
        {
            X_Rel = 0;
        }
        if (!(Y_Rel > Settings::Instance()->motionFilter || Y_Rel < Settings::Instance()->motionFilter * -1))
        {
            Y_Rel = 0;
        }

        // Pointer
        if (!(X_Rel == 0 && Y_Rel == 0))
        {
            cursorPositionCurrent = Server::Instance()->pointer->getPointerPosition();
            if (Y_Rel < 0) cursorPositionNew->setX(cursorPositionCurrent->x() - Y_Rel * Y_Rel);
            else if (Y_Rel > 0) cursorPositionNew->setX(cursorPositionCurrent->x() + Y_Rel * Y_Rel);
            else cursorPositionNew->setX(cursorPositionCurrent->x());

            if (X_Rel < 0) cursorPositionNew->setY(cursorPositionCurrent->y() - X_Rel * X_Rel);
            else if (X_Rel > 0) cursorPositionNew->setY(cursorPositionCurrent->y() + X_Rel * X_Rel);
            else cursorPositionNew->setY(cursorPositionCurrent->y());

            QList<QPoint*> *screenBounds = Screenshot::Instance()->getScreenBounds(Screenshot::Instance()->screenIndex);

            if (cursorPositionNew->x() > screenBounds->at(1)->x()) cursorPositionNew->setX(screenBounds->at(1)->x());
            if (cursorPositionNew->x() < screenBounds->at(0)->x()) cursorPositionNew->setX(screenBounds->at(0)->x());

            if (cursorPositionNew->y() > screenBounds->at(1)->y()) cursorPositionNew->setY(screenBounds->at(1)->y());
            if (cursorPositionNew->y() < screenBounds->at(0)->y()) cursorPositionNew->setY(screenBounds->at(0)->y());

            Server::Instance()->pointer->setPointerPosition(*new QPoint(cursorPositionNew->x(), cursorPositionNew->y()));
        }
    }
}

void MouseV2::calibratePointer(QByteArray &messageBytes)
{
    if (messageBytes.length() >= 5)
    {
        unsigned char firstByte = messageBytes.at(3);
        unsigned char secondByte = messageBytes.at(4);
        X_Def = firstByte;
        Y_Def = secondByte;

        if (X_Def > 99) X_Def = 100 - X_Def;
        if (Y_Def > 99) Y_Def = 100 - Y_Def;
    }
    else
    {
        X_Def = 0;
        Y_Def = 0;
    }
}

void MouseV2::parseLaser(QByteArray &messageBytes)
{
    QPoint *point_org, *point;
    point = new QPoint();
    point_org = commandGetPoint(messageBytes, 0);

    Server::Instance()->pointer->showPointer();

    QList<QPoint *> *locations = Screenshot::Instance()->getScreenBounds(Screenshot::Instance()->screenIndex);

    QPoint *startLocation = locations->at(0);
    QPoint *endLocation = locations->at(1);

    int width = endLocation->x() - startLocation->x();
    int height = endLocation->y() - startLocation->y();

    point->setX(point_org->x() * width / 255 + startLocation->x());
    point->setY(point_org->y() * height / 255 + startLocation->y());

    int x, y;

    x = (point->x() > startLocation->x() + 25) ? point->x() - 25 : startLocation->x();
    y = (point->y() > startLocation->y() + 25) ? point->y() - 25 : startLocation->y();

    Server::Instance()->pointer->setPointerPosition(*new QPoint(startLocation->x() + x, startLocation->y() + y));
    Server::Instance()->pointer->fadeOutPointer();
}

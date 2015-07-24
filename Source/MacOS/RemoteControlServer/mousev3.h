#ifndef MOUSEV3_H
#define MOUSEV3_H

#include "screenshot.h"
#include "touchpoint.h"

#include <ApplicationServices/ApplicationServices.h>
#include <QByteArray>
#include <QPoint>

class MouseV3
{
public:
    static MouseV3* Instance();

    void leftMouseDown();
    void leftMouseUp();
    void rightMouseDown();
    void rightMouseUp();
    void pointersDown();
    void pointersUp();
    void parsePointerData(QByteArray &data);
    void parseAbsolutePointerData(QByteArray &data, bool isPresenter);

private:
    static MouseV3* instance;
    MouseV3();

    struct Size
    {
        int Width;
        int Height;
    };

    QPoint* cursorPositionNew;
    CGPoint cursorPositionCurrent;

    QPoint *P_ORIGIN;
    QPoint *P1_New;
    QPoint *P1_Rel;
    QPoint *P1_Start, *P2_Start, *P3_Start;
    QPoint *P1_Last, *P2_Last, *P3_Last;
    float P3_Vector_Last, P3_Vector_Start;

    TouchPoint *pointDown;
    QList<TouchPoint*> *pointers;
    int downEventIndex;
    int moveEventIndex;

    bool mousePadDown;
    bool mouseLeftDown;
    bool mouseRightDown;

    int trim(int value, int min, int max);
    void moveMouseTo(int x, int y);
    void updatePointerPosition();
    void updateCursorPosition();
    void parseMultitouch();
    void checkForClick();
    CGPoint getCursorPosition();
};

#endif // MOUSEV3_H

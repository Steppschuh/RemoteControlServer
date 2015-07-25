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

    void leftMouseDown(bool isDoubleClick = false);
    void leftMouseUp(bool isDoubleClick = false);
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

    int scrollAmount;
    char currentGesture;
    enum
    {
        GESTURE_NONE = 0,
        GESTURE_ZOOM = 1,
        GESTURE_SCROLL = 2,

        CLICK_OFFSET_TOLERANCE = 10,
        CLICK_DELAY_TOLERANCE = 500,
        SCROLL_OFFSET_TOLERANCE = 50
    };

    QPoint *cursorPositionNew;
    CGPoint cursorPositionCurrent;

    QPoint *P_ORIGIN;
    QPoint *P1_New, *P2_New, *P3_New;
    QPoint *P1_Rel;
    QPoint *P1_Start, *P2_Start, *P3_Start;
    QPoint *P1_Last, *P2_Last, *P3_Last;
    float P3_Vector_New, P3_Vector_Last, P3_Vector_Start, P3_Vector_Event;

    TouchPoint *pointDown;
    QList<TouchPoint*> *pointers;
    int downEventIndex;
    int moveEventIndex;

    bool mousePadDown;
    bool mouseLeftDown;
    bool mouseRightDown;

    void leftSecondClick();
    int trim(int value, int min, int max);
    void moveMouseTo(int x, int y);
    void updatePointerPosition();
    void updateCursorPosition();
    void parseMultitouch();
    void processMultitouch();
    void checkForClick();
    CGPoint getCursorPosition();
    bool valueMatchesTolerance(float val1, float val2, int tolerance = CLICK_OFFSET_TOLERANCE);
    void mouseZoom(int direction, int zoomFactor);
    void mouseScrollVertical(int scrollDirection, int scrollLength);
};

#endif // MOUSEV3_H

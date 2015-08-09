#ifndef MOUSEV2_H
#define MOUSEV2_H

#include <QByteArray>
#include <QPoint>
#include <QString>

#ifdef Q_OS_MAC
    #include <ApplicationServices/ApplicationServices.h>
#endif

class MouseV2
{
public:
    static MouseV2 *Instance();

    float X_New, Y_New;
    float X_Rel, Y_Rel;
    float X_Def, Y_Def;

    void zoom(int value, int count = 1);
    void parseCursorMove(QByteArray &messageBytes);
    void parseCursorSet(QByteArray &messageBytes);
    void parseScroll(QByteArray &messageBytes);
    void parseClick(QByteArray &messageBytes);
    void parsePointer(QByteArray &messageBytes);
    void calibratePointer(QByteArray &messageBytes);
    void parseLaser(QByteArray &messageBytes);
    void leftClickRepeat(int count);

private:
    static MouseV2 *instance;
    MouseV2();

    enum
    {
        cmd_mouse_down = 0,
        cmd_mouse_up = 1,
        cmd_mouse_down_1 = 2,
        cmd_mouse_up_1 = 3,
        cmd_mouse_left_down = 5,
        cmd_mouse_left_up = 6,
        cmd_mouse_right_down = 7,
        cmd_mouse_right_up = 8,
        cmd_mouse_left_click = 11,
        cmd_mouse_right_click = 12
    };

    int scrollAmount;

    char currentGesture;

    enum
    {
        GESTURE_NONE = 0,
        GESTURE_ZOOM = 1,
        GESTURE_SCROLL = 2
    };

    QPoint *cursorPositionNew, *cursorPositionCurrent, *cursorPositionDown;

    QPoint *P_ORIGIN;
    QPoint *P1_New, *P2_New, *P3_New;
    QPoint *P1_Rel, *P2_Rel;
    QPoint *P1_Start, *P2_Start, *P3_Start;
    QPoint *P1_Last, *P2_Last, *P3_Last;
    float P3_Vector_New, P3_Vector_Start, P3_Vector_Last, P3_Vector_Event;

    long int dateLeftDown, dateLeftUp;
    long int P1_Down, P1_Up, P2_Down, P2_Up;

    bool mousePadDown, mouseLeftDown, mouseRightDown, isMultitouch;

    bool valueMatchesTolerance(float val1, float val2, int tolerance = 20);
    QPoint *commandGetPoint(QByteArray &messageBytes, int ID);
    bool isPointerDown(QPoint &point);
    void moveCursor();
    void processMultitouch();
};

#endif // MOUSEV2_H

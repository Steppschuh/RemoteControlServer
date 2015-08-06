#ifndef MOUSEMAC_H
#define MOUSEMAC_H

#include <QPoint>

#ifdef Q_OS_MAC
    #include <ApplicationServices/ApplicationServices.h>
#endif

class MouseMac
{
public:
    static MouseMac *Instance();

    QPoint *getCursorPosition();
    void leftMouseDown(bool isDoubleClick);
    void leftMouseUp(bool isDoubleClick);
    bool moveMouseTo(int x, int y);
    void mouseScrollVertical(int scrollDirection, int scrollLength);
    void rightMouseDown();
    void rightMouseUp();

private:
    static MouseMac *instance;
    MouseMac();

    CGPoint getCursorPositionCGPoint();
};

#endif // MOUSEMAC_H

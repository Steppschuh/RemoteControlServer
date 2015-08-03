#ifndef MOUSEV2_H
#define MOUSEV2_H

#include <QString>

class MouseV2
{
public:
    static MouseV2 *Instance();

    void mouseScrollHorizontal(int direction, int length);
    void parseMouse(QString cmd);
    void zoom(int value, int count = 1);

private:
    static MouseV2 *instance;
    MouseV2();
};

#endif // MOUSEV2_H

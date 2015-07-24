#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QList>
#include <QPoint>

class Screenshot
{
public:
    static Screenshot *Instance();

    int screenIndex;

    QList<QPoint*> *getScreenBounds(int index);

private:
    static Screenshot *instance;
    Screenshot();
};

#endif // SCREENSHOT_H

#include "screenshot.h"

#include <ApplicationServices/ApplicationServices.h>

#include <QObject>

Screenshot* Screenshot::instance = NULL;

Screenshot* Screenshot::Instance()
{
    if (!instance)
    {
        instance = new Screenshot();
    }
    return instance;
}

Screenshot::Screenshot()
{
    screenIndex = 0;
}

QList<QPoint *> *Screenshot::getScreenBounds(int index)
{
    // todo: support more than one screen
    QPoint *startLocation = new QPoint(0, 0);
    QPoint *endLocation = new QPoint(0, 0);

    CGDirectDisplayID displayID = CGMainDisplayID();
    endLocation->setX(CGDisplayPixelsWide(displayID));
    endLocation->setY(CGDisplayPixelsHigh(displayID));
    QList<QPoint*> *locations = new QList<QPoint*>();
    locations->append(startLocation);
    locations->append(endLocation);
    return locations;
}


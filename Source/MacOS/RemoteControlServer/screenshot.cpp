#include "screenshot.h"
#include "settings.h"

#include <ApplicationServices/ApplicationServices.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>

#include <QObject>

#include <QDebug>

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
    isSendingScreenshot = false;
    continueSendingScreenshots = false;

    lastRequestedWidth = 9999;
    lastRequestedQuality = Settings::Instance()->screenQualityFull;
}

QPixmap *Screenshot::getResizedScreenshot(int width)
{
    QPixmap *screenshot = getScreenshot(screenIndex);
    if (screenshot->width() > width)
    {
        screenshot = new QPixmap(screenshot->scaledToWidth(width));
    }

    if (Settings::Instance()->screenBlackWhite)
    {
        screenshot = convertToGrayscale(*screenshot);
    }
    return screenshot;
}

QPixmap *Screenshot::getScreenshot(int index)
{
    // todo support more than one screen
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screens.length() < index + 1)
    {
        index = 0;
    }
    QPixmap screenshot = screens.at(index)->grabWindow(index);
    return new QPixmap(screenshot);

}

QPixmap *Screenshot::convertToGrayscale(QPixmap &source)
{
    QImage image = source.toImage();
    image = image.convertToFormat(QImage::Format_Mono);
    return new QPixmap(QPixmap::fromImage(image));
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

void Screenshot::keepSendingScreenshots(Command &requestCommand, Command &responseCommand)
{

}

void Screenshot::sendScreenshot(bool full)
{

}

void Screenshot::toggleScreen()
{

}

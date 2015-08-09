#include "converter.h"
#include "screenshot.h"
#include "settings.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>

#include <QObject>

#include <QFile>
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

QPixmap *Screenshot::getScreenshot(bool fullscreen, int index)
{
    QPixmap *screenshot;

    screenshot = getScreenshot(index);

    if (fullscreen) Converter::Instance()->scalePixmap(screenshot, Settings::Instance()->screenScaleFull);
    else Converter::Instance()->scalePixmap(screenshot, Settings::Instance()->screenScale);

    if (Settings::Instance()->screenBlackWhite) screenshot = convertToGrayscale(*screenshot);

    return screenshot;
}

QPixmap *Screenshot::getResizedScreenshot(int width)
{
    width = (width > 1000) ? 1000 : width;

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
    QList<QPoint*> *locations = getScreenBounds(index);
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screens.length() < index + 1 || index < 0)
    {
        index = 0;
    }

    // When the second screen was right of the primary screen,
    // we had a bug when using the 0th screen as index in screens for taking the screenshot
    // We omit this bug now by using always the last screen and choosing the coordinates plus width/height of the screenshot
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap screenshot = screens.at(screens.length() - 1)->grabWindow(0,
                                                       locations->at(0)->x(), locations->at(0)->y(),
                                                       locations->at(1)->x() - locations->at(0)->x(),
                                                       locations->at(1)->y() - locations->at(0)->y());

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
    QList<QScreen*> screens = QGuiApplication::screens();
    QPoint *startLocation = new QPoint(0, 0);
    QPoint *endLocation = new QPoint(0, 0);

    if (index >= 0)
    {
        QScreen *currentScreen;
        if (index < screens.length()) currentScreen = screens.at(index);
        else
        {
            currentScreen = screens.at(0);
            screenIndex = 0;
        }

        startLocation->setX(currentScreen->geometry().x());
        startLocation->setY(currentScreen->geometry().y());
        endLocation->setX(currentScreen->geometry().x() + currentScreen->geometry().width());
        endLocation->setY(currentScreen->geometry().y() + currentScreen->geometry().height());
    }
    else
    {
        for (int i = 0; i < screens.length(); ++i)
        {
            QScreen *currentScreen = screens.at(i);

            if (currentScreen->geometry().x() < startLocation->x()) startLocation->setX(currentScreen->geometry().x());
            if (currentScreen->geometry().y() < startLocation->y()) startLocation->setY(currentScreen->geometry().y());

            if (currentScreen->geometry().x() + currentScreen->geometry().width() > endLocation->x())
                endLocation->setX(currentScreen->geometry().x() + currentScreen->geometry().width());
            if (currentScreen->geometry().y() + currentScreen->geometry().height() > endLocation->y())
                endLocation->setY(currentScreen->geometry().y() + currentScreen->geometry().height());
        }
    }
    QList<QPoint*> *locations = new QList<QPoint*>();
    locations->append(startLocation);
    locations->append(endLocation);
    return locations;
}

void Screenshot::keepSendingScreenshots(Command &requestCommand, Command &responseCommand)
{

}

void Screenshot::sendPixmap(QPixmap *pixmap, int quality)
{

}

void Screenshot::sendScreenshot(bool full)
{

}

void Screenshot::toggleScreen()
{
    int new_index = screenIndex + 1;
    if (new_index > QGuiApplication::screens().length() - 1) new_index = -1;
    screenIndex = new_index;
}

#include "apiv3.h"
#include "converter.h"
#include "network.h"
#include "remote.h"
#include "screenshot.h"
#include "serial.h"
#include "settings.h"

#include <iostream>
#include <thread>
#include <chrono>

#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QtConcurrent>
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
    averageColor = new QColor(Qt::white);
    lastAverageColor = new QColor(Qt::white);

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
    width = (width > 2000) ? 2000 : width;

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

void Screenshot::keepSendingScreenshots(Command &responseCommand)
{
    QtConcurrent::run(this, &Screenshot::keepSendingScreenshotsThread, responseCommand);
}

void Screenshot::keepSendingScreenshotsThread(Command &responseCommand)
{
    isSendingScreenshot = true;
    while (continueSendingScreenshots)
    {
        continueSendingScreenshots = false;
        ApiV3::Instance()->answerScreenGetRequest(responseCommand);
        std::this_thread::sleep_for (std::chrono::milliseconds(screenUpdateInterval));
        ApiV3::Instance()->answerScreenGetRequest(responseCommand);
        std::this_thread::sleep_for (std::chrono::milliseconds(screenUpdateInterval));
        ApiV3::Instance()->answerScreenGetRequest(responseCommand);
    }
    isSendingScreenshot = false;
}

void Screenshot::sendPixmap(QPixmap *pixmap, int quality)
{
    isSendingScreenshot = true;
    Command *command = new Command();
    command->source = Network::Instance()->getServerIp();
    command->destination = Remote::Instance()->lastCommand->source;
    command->priority = Command::PRIORITY_MEDIUM;
    command->data = Converter::Instance()->pixmapToByte(*pixmap, quality);
    command->send();
}

void Screenshot::sendScreenshot(bool full)
{
    if (!isSendingScreenshot)
    {
        if (full) sendPixmap(getScreenshot(true, screenIndex), Settings::Instance()->screenQualityFull);
        else sendPixmap(getScreenshot(false, screenIndex), Settings::Instance()->screenQuality);
    }
}

void Screenshot::toggleScreen()
{
    int new_index = screenIndex + 1;
    if (new_index > QGuiApplication::screens().length() - 1) new_index = -1;
    screenIndex = new_index;
}

void Screenshot::startUpdateColorTimer()
{
    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAverageColorTimerTick()));
    timer->start(colorUpdateInterval);
}

void Screenshot::updateAverageColorTimerTick()
{
    QtConcurrent::run(this, &Screenshot::updateAverageColor);
}

void Screenshot::updateAverageColor()
{
    if (Settings::Instance()->updateAmbientColor)
    {
        averageColor = getApproximateAverageColor(getScreenshot(false, screenIndex));
        if (Settings::Instance()->serialCommands && lastAverageColor->name() == averageColor->name())
        {
            QString message = "<22"
                    + QString::number(Converter::Instance()->byteToAsciiNumber(averageColor->red()))
                    + QString::number(Converter::Instance()->byteToAsciiNumber(averageColor->green()))
                    + QString::number(Converter::Instance()->byteToAsciiNumber(averageColor->blue()))
                    + "9>";
            Serial::Instance()->sendMessage(message);
            lastAverageColor = new QColor(averageColor->name());
        }
    }
}

QColor* Screenshot::getApproximateAverageColor(QPixmap *pixmap)
{
    int totalR = 0;
    int totalG = 0;
    int totalB = 0;
    int count = 1;
    QImage img = pixmap->toImage();
    for (int x = 0; x < pixmap->width(); ++x)
    {
        for (int y = 0; y < pixmap->height(); ++y)
        {
            QColor *pixel = new QColor(img.pixel(x, y));
            if (pixel->saturation() > 0.0 && pixel->value() > 0.0)
            {
                totalR += pixel->red();
                totalG += pixel->green();
                totalB += pixel->blue();
                count += 1;
            }
        }
    }

    int averageR = totalR / count;
    int averageG = totalG / count;
    int averageB = totalB / count;

    int roundUp = 200;
    int roundDown = 100;

    averageR = (averageR > roundUp) ? 255 : ((averageR < roundDown) ? 0 : averageR);
    averageG = (averageG > roundUp) ? 255 : ((averageG < roundDown) ? 0 : averageG);
    averageB = (averageB > roundUp) ? 255 : ((averageB < roundDown) ? 0 : averageB);

    return new QColor(averageR, averageG, averageB);
}

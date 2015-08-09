#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "command.h"

#include <QColor>
#include <QList>
#include <QPixmap>
#include <QPoint>

class Screenshot : public QObject
{
    Q_OBJECT

public:
    static Screenshot *Instance();

    int screenIndex = 0;
    bool isSendingScreenshot = false;
    bool continueSendingScreenshots = false;

    QColor *averageColor;
    QColor *lastAverageColor;

    int colorUpdateInterval = 2000; // in milliseconds

    int lastRequestedWidth = 9999;
    int lastRequestedQuality;

    QPixmap* getResizedScreenshot(int width);

    QList<QPoint*> *getScreenBounds(int index);
    void keepSendingScreenshots(Command &responseCommand);
    void sendScreenshot(bool full);
    void toggleScreen();
    void startUpdateColorTimer();

public slots:
    void updateAverageColorTimerTick();

private:
    static Screenshot *instance;
    Screenshot();

    const int screenUpdateInterval = 200;

    QPixmap* getScreenshot(bool fullscreen, int index);
    QPixmap* getScreenshot(int index);
    QPixmap* convertToGrayscale(QPixmap &source);
    void keepSendingScreenshotsThread(Command &responseCommand);
    void sendPixmap(QPixmap *pixmap, int quality);
    void updateAverageColor();
    QColor* getApproximateAverageColor(QPixmap *pixmap);

};

#endif // SCREENSHOT_H

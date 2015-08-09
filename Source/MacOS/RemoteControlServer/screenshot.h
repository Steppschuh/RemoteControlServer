#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "command.h"

#include <QList>
#include <QPixmap>
#include <QPoint>

class Screenshot
{
public:
    static Screenshot *Instance();

    int screenIndex;
    bool isSendingScreenshot;
    bool continueSendingScreenshots;

    int lastRequestedWidth;
    int lastRequestedQuality;

    QPixmap* getResizedScreenshot(int width);

    QList<QPoint*> *getScreenBounds(int index);
    void keepSendingScreenshots(Command &requestCommand, Command &responseCommand);
    void sendScreenshot(bool full);
    void toggleScreen();

private:
    static Screenshot *instance;
    Screenshot();

    QPixmap* getScreenshot(bool fullscreen, int index);
    QPixmap* getScreenshot(int index);
    QPixmap* convertToGrayscale(QPixmap &source);
    void sendPixmap(QPixmap *pixmap, int quality);
    void startUpdateColorTimer();
    void updateAverageColorTimerTick();
    void updateAverageColor();
    void getAverageColor(QPixmap *pixmap); // todo return type
    void getApproximateAverageColor(QPixmap *pixmap); // todo return type

};

#endif // SCREENSHOT_H

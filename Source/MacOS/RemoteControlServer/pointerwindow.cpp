#include "mousev2.h"
#include "pointerwindow.h"
#include "settings.h"

#ifdef Q_OS_MAC
    #include "mousemac.h"
#endif

#include <QLabel>
#include <QPainter>

#include <QDebug>

PointerWindow::PointerWindow(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent, f)
{
    setFixedSize(100, 100);
    setAttribute(Qt::WA_TranslucentBackground);
    pointerImage = new QPixmap(":/Resources/pointer_white.png");
    setPixmap(*pointerImage);
}

void PointerWindow::showPointer()
{
    switch (Settings::Instance()->pointerDesign)
    {
    case 0:
        pointerImage = new QPixmap(":/Resources/pointer_white.png");
        break;
    case 1:
        pointerImage = new QPixmap(":/Resources/pointer.png");
    }
    isVisible = true;
    setOpacity(1.0);
    show();
}

void PointerWindow::hidePointer()
{
    hide();
    setOpacity(0.0);
    isVisible = false;
}

void PointerWindow::setOpacity(float o)
{
    o = (o > 1.0) ? 1.0 : ((o < 0.0) ? 0.0 : o);
    opacity = o;

    QImage image(pointerImage->size(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter p(&image);
    p.setOpacity(o);
    p.drawPixmap(0, 0, *pointerImage);
    p.end();

    setPixmap(QPixmap::fromImage(image));
}

void PointerWindow::startFadeOutPointer()
{
    if (isFadingOutTimed) fadeOutDelayTimer->stop();

    lowerOpacityTimer = new QTimer();
    connect(lowerOpacityTimer, SIGNAL(timeout()), this, SLOT(lowerOpacity()));
    lowerOpacityTimer->start(100);

    fadeOutDelayTimer->stop();

    if (Settings::Instance()->clickOnLaserUp)
    {
        hide();
        MouseV2::Instance()->leftClickRepeat(1);
    }

    isVisible = false;
}

void PointerWindow::lowerOpacity()
{
    if (opacity != 0.0)
    {
        isFadingOut = true;
        setOpacity(opacity - 0.05);
    }
    else
    {
        isFadingOut = false;
        lowerOpacityTimer->stop();
        isVisible = false;
        hide();
    }
}

void PointerWindow::fadeOutPointer()
{
    if (isFadingOutTimed) fadeOutDelayTimer->stop();
    if (isFadingOut) lowerOpacityTimer->stop();

    fadeOutDelayTimer = new QTimer();
    connect(fadeOutDelayTimer, SIGNAL(timeout()), this, SLOT(startFadeOutPointer()));

    if (Settings::Instance()->clickOnLaserUp) fadeOutDelayTimer->start(1000);
    else fadeOutDelayTimer->start(2000);

    isFadingOutTimed = true;
}

void PointerWindow::setPointerPosition(QPoint &point)
{
    move(point.x(), point.y());

    if (Settings::Instance()->clickOnLaserUp)
    {
        int x = point.x() + (geometry().width() / 2);
        int y = point.y() + (geometry().height() / 2);

#ifdef Q_OS_MAC
        MouseMac::Instance()->moveMouseTo(x, y);
#endif
    }
}

QPoint* PointerWindow::getPointerPosition()
{
    return new QPoint(geometry().x(), geometry().y());
}


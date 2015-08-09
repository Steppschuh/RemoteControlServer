#ifndef POINTERWINDOW_H
#define POINTERWINDOW_H

#include <QLabel>
#include <QTimer>

class PointerWindow : public QLabel
{
    Q_OBJECT

public:
    bool isVisible = false;

    PointerWindow(QWidget * parent = 0, Qt::WindowFlags f = 0);
    void showPointer();
    void hidePointer();
    void fadeOutPointer();
    void setPointerPosition(QPoint &point);
    QPoint *getPointerPosition();

public slots:
    void lowerOpacity();
    void startFadeOutPointer();

private:
    bool isFadingOut = false;
    bool isFadingOutTimed = false;

    QTimer *fadeOutDelayTimer;
    QTimer *lowerOpacityTimer;

    QPixmap *pointerImage;

    float opacity = 0;

    void setOpacity(float o);
};

#endif // POINTERWINDOW_H

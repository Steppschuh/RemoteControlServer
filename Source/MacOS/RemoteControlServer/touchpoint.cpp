#include "touchpoint.h"

#include <QDateTime>

TouchPoint::TouchPoint()
{
    timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
}


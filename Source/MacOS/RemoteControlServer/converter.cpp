#include "converter.h"

#include "math.h"

#include <QBuffer>

#include <QDebug>

Converter* Converter::instance = NULL;

Converter* Converter::Instance()
{
    if (!instance)
    {
        instance = new Converter();
    }
    return instance;
}

Converter::Converter()
{
}

void Converter::scalePixmap(QPixmap *sourcePixmap, float scale)
{
    if (scale != 1 && scale > 0)
    {
        QPixmap *destinationPixmap = new QPixmap(sourcePixmap->scaled(sourcePixmap->width() * scale, sourcePixmap->height() * scale));
        sourcePixmap = destinationPixmap;
    }
}

QByteArray *Converter::pixmapToByte(QPixmap &pixmap, int compression)
{
    QByteArray *bytes = new QByteArray();
    QBuffer buffer(bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG", compression);
    return bytes;
}

QString Converter::byteToString(QByteArray &value, int index)
{
    return value.right(value.length() - index);
}

QString Converter::boolToString(bool value)
{
    return value ? "1" : "0";
}

bool Converter::stringToBool(QString &value)
{
    return (value == "1") ? true : false;
}

QString Converter::commandToString(Command &command)
{
    QString commandString = "[ ";
    for (int i = 0; i < command.data->length(); ++i)
    {
        commandString += QString(command.data->at(i)) + " ";
    }
    commandString += "]";
    return commandString;
}

float Converter::getPointDistance(QPoint &P1, QPoint &P2, int digits)
{
    float d = 0;
    d = sqrt(pow(P1.x() - P2.x(), 2) + pow(P1.y() - P2.y(), 2));
    d = round(d * pow(10, digits)) / pow(10, digits);
    return d;
}

char Converter::byteToAsciiNumber(char b)
{
    int value = 0;
    value = (b * 9) / 255;
    return (char) value;
}

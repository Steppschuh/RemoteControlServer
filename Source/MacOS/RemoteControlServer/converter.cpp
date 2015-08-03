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

Converter::Converter() :
    maxSizeForBitmaps(100000)
{
}

QByteArray *Converter::bitmapToByte(QPixmap &bitmap, int compression)
{
    QByteArray *bytes = new QByteArray();
    QBuffer buffer(bytes);
    buffer.open(QIODevice::WriteOnly);
    bitmap.save(&buffer, "JPG", compression);
    qDebug() << bytes->length();
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
        commandString += command.data->at(i) + " ";
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

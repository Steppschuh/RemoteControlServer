#ifndef CONVERTER_H
#define CONVERTER_H

#include <command.h>

#include <QByteArray>
#include <QPixmap>
#include <QPoint>
#include <QString>

class Converter
{
public:
    static Converter *Instance();

    QByteArray* bitmapToByte(QPixmap &bitmap, int compression);
    QString byteToString(QByteArray &value, int index);
    QString boolToString(bool value);
    bool stringToBool(QString &value);
    QString commandToString(Command &command);
    float getPointDistance(QPoint &P1, QPoint &P2, int digits = 2);

private:
    Converter();
    static Converter *instance;
};

#endif // CONVERTER_H

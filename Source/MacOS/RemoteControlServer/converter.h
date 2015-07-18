#ifndef CONVERTER_H
#define CONVERTER_H

#include <QByteArray>
#include <QString>

class Converter
{
public:
    static Converter *Instance();

    QString byteToString(QByteArray &value, int index);
    QString boolToString(bool value);
    bool stringToBool(QString &value);
//    QByteArray *buildCommandData(QByteArray)

private:
    Converter();
    static Converter *instance;
};

#endif // CONVERTER_H

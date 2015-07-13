#ifndef CONVERTER_H
#define CONVERTER_H

#include <QString>

class Converter
{
public:
    static Converter *Instance();

    QString boolToString(bool value);
    bool stringToBool(QString value);

private:
    Converter();
    static Converter *instance;
};

#endif // CONVERTER_H

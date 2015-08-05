#include "mousev2.h"

MouseV2 *MouseV2::instance = NULL;

MouseV2 *MouseV2::Instance()
{
    if (!instance)
    {
        instance = new MouseV2();
    }
    return instance;
}

MouseV2::MouseV2()
{
}

void MouseV2::mouseScrollHorizontal(int direction, int length)
{

}

void MouseV2::parseMouse(QString cmd)
{

}

void MouseV2::zoom(int value, int count)
{

}

void MouseV2::parseCursorMove(QByteArray &messageBytes)
{

}

void MouseV2::parseCursorSet(QByteArray &messageBytes)
{

}

void MouseV2::parseScroll(QByteArray &messageBytes)
{

}

void MouseV2::parseClick(QByteArray &messageBytes)
{

}

void MouseV2::parsePointer(QByteArray &messageBytes)
{

}

void MouseV2::calibratePointer(QByteArray &messageBytes)
{

}

void MouseV2::parseLaser(QByteArray &messageBytes)
{

}

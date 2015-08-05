#ifndef MOUSEV2_H
#define MOUSEV2_H

#include <QByteArray>
#include <QString>

class MouseV2
{
public:
    static MouseV2 *Instance();

    float X_New;
    float X_Rel;
    float X_Def;

    void mouseScrollHorizontal(int direction, int length);
    void parseMouse(QString cmd);
    void zoom(int value, int count = 1);
    void parseCursorMove(QByteArray &messageBytes);
    void parseCursorSet(QByteArray &messageBytes);
    void parseScroll(QByteArray &messageBytes);
    void parseClick(QByteArray &messageBytes);
    void parsePointer(QByteArray &messageBytes);
    void calibratePointer(QByteArray &messageBytes);
    void parseLaser(QByteArray &messageBytes);

private:
    static MouseV2 *instance;
    MouseV2();
};

#endif // MOUSEV2_H

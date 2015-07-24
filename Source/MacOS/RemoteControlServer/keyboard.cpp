#include "keyboard.h"
#include <QObject>

Keyboard *Keyboard::instance = NULL;

Keyboard *Keyboard::Instance()
{
    if (!instance)
    {
        instance = new Keyboard();
    }
    return instance;
}

Keyboard::Keyboard()
{

}


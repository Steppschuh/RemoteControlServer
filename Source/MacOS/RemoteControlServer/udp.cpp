#include "command.h"
#include "udp.h"

UDP* UDP::instance = NULL;

UDP* UDP::Instance()
{
    if (!instance)
    {
        instance = new UDP();
    }
    return instance;
}

UDP::UDP()
{

}

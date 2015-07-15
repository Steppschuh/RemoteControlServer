#include "remote.h"

Remote* Remote::instance = NULL;

Remote* Remote::Instance()
{
    if (!instance)
    {
        instance = new Remote();
    }
    return instance;
}

Remote::Remote()
{
//    latestApi = 3;
    lastCommand = new Command();
}

//void Remote::processCommand(Command &command)
//{
//    lastCommand = &command;
//}

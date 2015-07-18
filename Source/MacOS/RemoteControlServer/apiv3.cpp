#include "apiv3.h"

ApiV3* ApiV3::instance = NULL;

ApiV3* ApiV3::Instance()
{
    if (!instance)
    {
        instance = new ApiV3();
    }
    return instance;
}

ApiV3::ApiV3()
{

}

bool ApiV3::isBroadcast(Command &command)
{
    return false;
}

bool ApiV3::isConnectionCommand(Command &command)
{
    return false;
}

void ApiV3::parseCommand(Command *command)
{

}

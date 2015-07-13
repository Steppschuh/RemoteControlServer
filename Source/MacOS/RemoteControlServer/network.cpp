#include "network.h"

Network* Network::instance = NULL;

Network* Network::Instance()
{
    if (!instance)
    {
        instance = new Network();
    }
    return instance;
}

Network::Network()//:
//    localHost("127.0.0.1")
{

}


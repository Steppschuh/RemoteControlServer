#include "tcp.h"

TCP* TCP::instance = NULL;

TCP* TCP::Instance()
{
    if (!instance)
    {
        instance = new TCP();
    }
    return instance;
}

TCP::TCP() //:
//    portReceive(1925),
//    portSend(1927),
//    buffer(1024),
//    receiveTimeout(2000),
//    retries(3),
//    retryTimeout(1100),
//    sendTimeout(1000)
{
//    isListening = false;

//    Logger::Instance()->add("Initializing TCP");
}


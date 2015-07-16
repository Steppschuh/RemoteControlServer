#include "authentication.h"
#include "settings.h"

Authentication* Authentication::instance = NULL;

Authentication* Authentication::Instance()
{
    if (!instance)
    {
        instance = new Authentication();
    }
    return instance;
}

Authentication::Authentication()
{

}

bool Authentication::isProtected()
{
    return (Settings::Instance()->usePin || Settings::Instance()->useWhitelist) ? true : false;
}

bool Authentication::isAuthenticated(QString ip, QString pin)
{
    if (!isProtected())
    {
        return true;
    }
    else
    {
        return ((Settings::Instance()->useWhitelist && isWhitelisted(ip)) || (Settings::Instance()->usePin && Settings::Instance()->pin == pin))
                ? true : false;

    }
}

bool Authentication::isWhitelisted(QString ip)
{
    return (Settings::Instance()->whitelistedIps->contains(ip)) ? true : false;
}

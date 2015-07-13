#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QString>

#include "app.h"

class Authentication
{
public:
    Authentication *Instance();

private:
    Authentication();
    Authentication *instance;

//    bool isProtected();
//    bool isAuthenticated(QString ip, QString pin);
//    bool isWhitelisted(QString ip);
//    bool isWhitelisted(App *app);
//    void addToWhiteList(QString ip);
//    void removeFromWhiteList(QString ip);
};

#endif // AUTHENTICATION_H

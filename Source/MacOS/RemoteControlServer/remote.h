#ifndef REMOTE_H
#define REMOTE_H

#include "command.h"

class Remote
{
public:
    Remote *Instance();

private:
    Remote();
    Remote *instance;

//    char latestApi;
//    Command *lastCommand;

//    void processCommand(Command &command);
};

#endif // REMOTE_H

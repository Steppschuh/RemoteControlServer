#ifndef REMOTE_H
#define REMOTE_H

#include "command.h"

class Remote
{
public:
    static Remote *Instance();

    Command *lastCommand;


private:
    Remote();
    static Remote *instance;

//    char latestApi;

//    void processCommand(Command &command);
};

#endif // REMOTE_H

#ifndef REMOTE_H
#define REMOTE_H

#include "command.h"

class Remote
{
public:
    static Remote *Instance();

    char latestApi;
    Command *lastCommand;


private:
    Remote();
    static Remote *instance;

//    void processCommand(Command &command);
};

#endif // REMOTE_H

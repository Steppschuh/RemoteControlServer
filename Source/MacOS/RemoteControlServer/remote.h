#ifndef REMOTE_H
#define REMOTE_H

#include "command.h"

class Remote
{
public:
    static Remote *Instance();

    char latestApi;
    Command *lastCommand;

    void processCommand(Command &command);

private:
    Remote();
    static Remote *instance;
};

#endif // REMOTE_H

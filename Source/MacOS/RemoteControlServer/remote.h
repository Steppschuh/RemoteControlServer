#ifndef REMOTE_H
#define REMOTE_H

#include <command.h>

class Remote
{
public:
    Remote();

    char latestApi;

    void processCommand(Command &command);

private:
    Command *lastCommand;
};

#endif // REMOTE_H

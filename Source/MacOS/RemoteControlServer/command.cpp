#include "command.h"

const char Command::PRIORITY_LOW = 0;
const char Command::PRIORITY_MEDIUM = 1;
const char Command::PRIORITY_HIGH = 2;
const char Command::PRIORITY_INDISPENSABLE = 3;

Command::Command()
{
    source = "Unknown";
    destination = "Unknown";
    priority = 0;
}


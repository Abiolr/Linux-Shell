#include "constants.h"

#ifndef COMMAND_H
#define COMMAND_H

struct Command {
     char argv[MAX_ARGS + 1][BUFFER_SIZE];
     unsigned int argc;
     int background;              /* 0=foreground, otherwise=background */
};

void get_command(struct Command * command);
void run_command(struct Command * command);

#endif
#include "constants.h"

#ifndef COMMAND_H
#define COMMAND_H

typedef struct {
     char argv[MAX_ARGS][BUFFER_SIZE];     /* an array of pointers to (dyn-alloc'ed) strings */
     unsigned int argc;
} Command;

void get_command(Command * command);
void run_command(Command * command);

#endif
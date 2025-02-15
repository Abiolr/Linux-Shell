#include "command.h"
#include "str_lib.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_command(Command *command)
{
    char buffer[BUFFER_SIZE];
    int bytes_read;
    write(2, "mysh$ ", 6);
    bytes_read = read(0, buffer, BUFFER_SIZE);
    buffer[bytes_read - 1] = '\0';
    command->argc = 0;

    tokenizeString(buffer, command->argv, &command->argc);
    //print_tokens(command->argv, &command->argc);
}

/*
void run_command(Command *command)
{
    int pid;
    int child_status;

    char * const envp[] = { NULL };
    pid = fork();
    
    if (pid == 0) 
    {
        execve(command->argv[0], command->argv, envp);
    }

    waitpid(pid, &child_status, 0);
}
*/
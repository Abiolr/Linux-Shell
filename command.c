#include "command.h"
#include "str_lib.h"
#include "mylib.h"

void get_command(struct Command *command)
{
    char buffer[MAX_COMMAND_LENGTH];
    int bytes_read;
    write(2, "mysh$ ", 6);
    bytes_read = read(0, buffer, MAX_COMMAND_LENGTH);
    if (bytes_read <= 0) {
        _exit(0);
    }
    
    int length = bytes_read;
    for (int i = 0; i < bytes_read; i++) {
        if (buffer[i] == '\n') {
            length = i;
            break;
        }
    }
    buffer[length] = '\0';
    command->argc = 0;
    my_free_all();
    tokenizeString(buffer, command->argv, &command->argc);
}


void run_command(struct Command *command)
{
    int pid;
    int child_status;

    char * const envp[] = {NULL};
    pid = fork();
    
    if (pid == 0) 
    {
        execve(command->argv[0], command->argv, envp);
    }

    waitpid(pid, &child_status, 0);

    if (my_streq(command->argv[0],"/bin/cat"))
        write(2, "\n", 1);
}
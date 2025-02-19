#include "command.h"
#include "str_lib.h"
#include "mylib.h"

void get_command(struct Command *command)
{
    char buffer[MAX_COMMAND_LENGTH];
    int bytes_read;
    int length;
    command->background = 0;
    write(2, "mysh$ ", 6);
    bytes_read = read(0, buffer, MAX_COMMAND_LENGTH - 1);
    if (bytes_read == -1)
    {
        write(2, "error: buffer size exceeded", 28);
        _exit(0);
    }
    else if (bytes_read <= 0)
    {
        _exit(0);
    }
    
    length = bytes_read;
    for (int i = 0; i < bytes_read; i++)
    {
        if (buffer[i] == '\n')
        {
            length = i;
            break;
        }
    }
    buffer[length] = '\0';

    if (buffer[length-1] == '&')
    {
        command->background = 1;
        buffer[length-1] = '\0';
    }
        
    command->argc = 0;
    my_free_all();
    tokenizeString(buffer, command->argv, &command->argc);
}

void run_command(struct Command *command)
{
    pid_t pid;
    int child_status;

    char * const envp[] = {NULL};
    pid = fork();
    
    if (pid == 0) 
    {
        int command_ran = execve(command->argv[0], command->argv, envp);
        if (command_ran == -1)
        {
            write(2, "command not found: ", 20);
            write(2, command->argv[0], my_strlen(command->argv[0]));
            write(2, "\n", 1);
            _exit(1);
        }
    }
    else if (pid > 0)
    {
        if (command->background == 0)
        {
            waitpid(pid, &child_status, 0);
        }

        if (my_streq(command->argv[0],"/bin/cat"))
        {
            write(2, "\n", 1);
        }
    }
    else
    {
        write(2, "fork failed\n", 12);
    }
}
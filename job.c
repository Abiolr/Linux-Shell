#include "job.h"

void get_job(struct Job *job)
{
    char *parse_infile[MAX_ARGS + 1];
    char *parse_outfile[MAX_ARGS + 1];
    char *commands[MAX_ARGS + 1];
    unsigned int num_tokens;

    char buffer[MAX_COMMAND_LENGTH];
    int bytes_read;
    int length;
    job->background = 0;
    write(2, "mysh$ ", 6);
    bytes_read = read(0, buffer, MAX_COMMAND_LENGTH - 1);
    if (bytes_read <= 0)
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
        job->background = 1;
        buffer[length-1] = '\0';
    }
        
    num_tokens = 0;
    my_free_all();
    tokenizeString(buffer, parse_infile, &num_tokens, '<');

    if (num_tokens == 2)
    {
        job->infile_path = parse_infile[0];
        my_strncpy(buffer, parse_infile[1], my_strlen(parse_infile[1]));
    }

    else if (num_tokens == 1)
    {
        job->infile_path = NULL;
        my_strncpy(buffer, parse_infile[0], my_strlen(parse_infile[0]));
    }

    else
    {
        write(2, "error: too many infile paths\n", 30);
        _exit(0);
    }
   
    tokenizeString(buffer, parse_outfile, &num_tokens, '>');
    if (num_tokens == 2)
    {
        job->outfile_path = parse_outfile[1];    
    }

    else if (num_tokens == 1)
    {
        job->outfile_path = NULL;
    }

    else
    {
        write(2, "error: too many outfile paths\n", 31);
        _exit(0);
    }
    my_strncpy(buffer, parse_outfile[0], my_strlen(parse_outfile[0]));

    tokenizeString(buffer, commands, &num_tokens, '|');

    if (num_tokens == 2)
    {
        tokenizeString(commands[0], job->pipeline[0].argv, &job->pipeline->argc, ' ');
        tokenizeString(commands[1], job->pipeline[1].argv, &job->pipeline->argc, ' ');
        job->num_stages = 2;
    }
    else if (num_tokens == 1)
    {
        tokenizeString(commands[0], job->pipeline[0].argv, &job->pipeline->argc, ' ');
        job->num_stages = 2;
    }

    else
    {
        write(2, "error: too many commands\n", 26);
        _exit(0);
    }

    //print_tokens(job->pipeline[0].argv, &job->pipeline->argc);
}

void run_job(struct Job *job)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int child_status;
    char * const envp[] = {NULL};

    if (pipe(pipefd) == -1)
    {
        write(2, "pipe failed\n", 12);
        _exit(0);
    }
    
    pid1 = fork();
    
    if (pid1 == 0) 
    {
        int command_ran = execve(job->pipeline[0].argv[0], job->pipeline[0].argv, envp);
        close(pipefd[READ_END]);
        dup2(pipefd[WRITE_END], 1);
        
        if (command_ran == -1)
        {
            write(2, "command not found: ", 20);
            write(2, job->pipeline[0].argv[0], my_strlen(job->pipeline[0].argv[0]));
            write(2, "\n", 1);
            _exit(1);
        }
    }
    else if (pid1 > 0)
    {
        if (job->background == 0)
        {
            waitpid(pid1, &child_status, 0);
        }

        if (my_streq(job->pipeline[0].argv[0], "/bin/cat"))
        {
            write(2, "\n", 1);
        }
    }
    else
    {
        write(2, "fork failed\n", 12);
    }
}
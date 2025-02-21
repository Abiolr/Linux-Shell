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

    if (length > 0 && buffer[length - 1] == '&')
    {
        job->background = 1;
        buffer[length - 1] = '\0';
    }

    num_tokens = 0;
    my_free_all();
    tokenize_string(buffer, parse_infile, &num_tokens, '<');

    if (num_tokens == 2)
    {
        job->infile_path = parse_infile[0];
        get_valid_string(job->infile_path);
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

    tokenize_string(buffer, parse_outfile, &num_tokens, '>');
    if (num_tokens == 2)
    {
        job->outfile_path = parse_outfile[1];
        get_valid_string(job->outfile_path);
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

    tokenize_string(buffer, commands, &num_tokens, '|');
    job->num_stages = num_tokens;

    for (int i = 0; i < num_tokens; i++)
    {
        tokenize_string(commands[i], job->pipeline[i].argv, &job->pipeline[i].argc, ' ');
    }
}

void run_job(struct Job *job)
{
    int pipefd[2 * (job->num_stages - 1)];
    pid_t pids[job->num_stages];
    int child_status;

    if (job->num_stages > 1)
    {
        for (int i = 0; i < job->num_stages - 1; i++)
        {
            if (pipe(pipefd + 2 * i) == -1)
            {
                write(2, "pipe failed\n", 12);
                _exit(1);
            }
        }
    }

    for (int i = 0; i < job->num_stages; i++)
    {
        pids[i] = fork();

        if (pids[i] == 0)
        {
            if (i == 0 && job->infile_path != NULL)
            {
                int infile_fd = open(job->infile_path, O_RDONLY);
                if (infile_fd == -1)
                {
                    write(2, "error: cannot open input file\n", 29);
                    _exit(1);
                }
                dup2(infile_fd, STDIN_FILENO);
                close(infile_fd);
            }

            if (i == job->num_stages - 1 && job->outfile_path != NULL)
            {
                int outfile_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (outfile_fd == -1)
                {
                    write(2, "error: cannot open output file\n", 31);
                    _exit(1);
                }
                dup2(outfile_fd, STDOUT_FILENO);
                close(outfile_fd);
            }

            if (job->num_stages > 1)
            {
                if (i > 0)
                {
                    dup2(pipefd[2 * (i - 1)], STDIN_FILENO);
                }

                if (i < job->num_stages - 1)
                {
                    dup2(pipefd[2 * i + 1], STDOUT_FILENO);
                }

                for (int j = 0; j < 2 * (job->num_stages - 1); j++)
                {
                    close(pipefd[j]);
                }
            }

            int command_ran = execve(job->pipeline[i].argv[0], job->pipeline[i].argv, NULL);
            if (command_ran == -1)
            {
                write(2, "command not found: ", 20);
                write(2, job->pipeline[i].argv[0], my_strlen(job->pipeline[i].argv[0]));
                write(2, "\n", 1);
                _exit(1);
            }
        }
        else if (pids[i] < 0)
        {
            write(2, "fork failed\n", 12);
            _exit(1);
        }
    }

    if (job->num_stages > 1)
    {
        for (int i = 0; i < 2 * (job->num_stages - 1); i++)
        {
            close(pipefd[i]);
        }
    }

    if (job->background == 0)
    {
        for (int i = 0; i < job->num_stages; i++)
        {
            waitpid(pids[i], &child_status, 0);
        }
    }
}

void get_valid_string(char *str)
{
    int i = 0;
    int in_word = 0;
    char *start = str;

    while (str[i] != '\0')
    {
        if (str[i] == ' ')
        {
            if (in_word)
            {
                write(2, "error: path must not contain spaces between letters\n", 53);
                _exit(0);
            }
        }
        else
        {
            in_word = 1;
        }
        i++;
    }

    while (*start == ' ')
    {
        start++;
    }

    char *end = str;
    while (*end != '\0')
    {
        end++;
    }
    end--;
    while (end > start && *end == ' ')
    {
        end--;
    }
    *(end + 1) = '\0';

    if (start != str) {
        i = 0;
        while (start[i] != '\0')
        {
            str[i] = start[i];
            i++;
        }
        str[i] = '\0';
    }
}
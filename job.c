/*
 * job.c
 *
 * This file contains functions for managing and executing shell jobs.
 * It includes functions for parsing user input, setting up pipelines, handling
 * I/O redirections, and executing commands. The main data structure is `struct Job`,
 * which holds information about the command pipeline, input/output files, and
 * whether the job should run in the background.
 */
#include "job.h"

/*
 * get_job
 *
 * Purpose: 
 *    Reads input from the user, parses it to determine input/output redirections, 
 *    background execution, and command pipelines, and stores the results 
 *    in a Job structure.
 *
 * Input:
 *   - job: Pointer to a Job structure to be populated with parsed information.
 *
 * Output:
 *   - job: The Job structure is filled with parsed command information, including
 *          input/output file paths, background flag, and command stages.
 *
 * Assumptions/Limitations:
 *   - Assumes the input buffer is no longer than MAX_COMMAND_LENGTH.
 *   - Assumes the input is well-formed (e.g no more than one input/output redirection).
 *   - Does not handle complex command syntax (e.g nested redirections).
 */
void get_job(struct Job *job) {
    char *parse_infile[MAX_ARGS + 1];
    char *parse_outfile[MAX_ARGS + 1];
    unsigned int num_tokens;

    char buffer[MAX_COMMAND_LENGTH];
    int length;
    job->background = 0;

    my_free_all();
    read_input(buffer, &length);

    if (length == 0)
    {
        job->infile_path = NULL;
        job->outfile_path = NULL;
        job->num_stages = 0;
        return;
    }

    if (length > 0 && buffer[length - 1] == '&')
    {
        job->background = 1;
        buffer[length - 1] = '\0';
    }

    num_tokens = 0;
    tokenize_string(buffer, parse_infile, &num_tokens, '<');

    if (num_tokens == 2)
    {
        job->infile_path = parse_infile[1];
        get_valid_string(job->infile_path);
        my_strncpy(buffer, parse_infile[0], my_strlen(parse_infile[0]));
    }
    
    else if (num_tokens == 1)
    {
        if (parse_infile[0][0] == '<')
        {
            job->infile_path = parse_infile[0] + 1;
            get_valid_string(job->infile_path);
            buffer[0] = '\0';
        }
        else
        {
            job->infile_path = NULL;
            my_strncpy(buffer, parse_infile[0], my_strlen(parse_infile[0]));
        }
    }
    else
    {
        write(2, "error: too many infile paths\n", 30);
        return;
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
        return;
    }
    my_strncpy(buffer, parse_outfile[0], my_strlen(parse_outfile[0]));
    
    parse_commands(buffer, job);
}

/*
 * run_job
 *
 * Purpose: 
 *    Runs the commands in the Job structure, handling I/O redirections 
 *    and piping between commands.
 *
 * Input:
 *   - job: Pointer to a Job structure containing the command pipeline to execute.
 *
 * Assumptions/Limitations:
 *   - Assumes the Job structure has been properly populated by get_job.
 *   - Assumes the system has enough resources to fork processes and create pipes.
 *   - Does not handle errors in command execution gracefully (e.g. missing commands).
 */
void run_job(struct Job *job)
{
    int pipefd[2 * (job->num_stages - 1)];
    pid_t pids[job->num_stages];
    int child_status;

    if (job->num_stages > 1) setup_pipes(pipefd, job->num_stages);

    for (int i = 0; i < job->num_stages; i++)
    {
        pids[i] = fork();

        if (pids[i] == 0)
        {
            handle_redirections(job, i, pipefd);
            close_pipes(pipefd, job->num_stages);
            execute_command(job, i);
        }

        else if (pids[i] < 0)
        {
            write(2, "fork failed\n", 12);
            return;
        }
    }

    if (job->num_stages > 1) close_pipes(pipefd, job->num_stages);
    if (job->background == 0)
    {
        for (int i = 0; i < job->num_stages; i++)
        {
            waitpid(pids[i], &child_status, 0);
        }
    }
}

/*
 * read_input
 *
 * Purpose: 
 *    Reads a line of input from the user and stores it in a buffer.
 *
 * Inputs:
 *   - buffer: Pointer to a buffer where the input will be stored.
 *   - length: Pointer to an integer where the length of the input will be stored.
 *
 * Outputs:
 *   - buffer: Contains the user input.
 *   - length: Contains the length of the input (excluding the newline character).
 *
 * Assumptions/Limitations:
 *   - Assumes the buffer is large enough to hold the input (MAX_COMMAND_LENGTH - 1).
 *   - Assumes the input is terminated by a newline character.
 */
void read_input(char *buffer, int *length)
{
    write(2, "mysh$ ", 6);
    int bytes_read = read(0, buffer, MAX_COMMAND_LENGTH - 1);

    if (bytes_read <= 0)
    {
        _exit(0);
    }

    *length = bytes_read;
    for (int i = 0; i < bytes_read; i++)
    {
        if (buffer[i] == '\n')
        {
            *length = i;
            break;
        }
    }
    buffer[*length] = '\0';
}

/*
 * parse_commands
 *
 * Purpose: 
 *    Splits a command string into individual commands based on the pipe character ('|').
 *
 * Inputs:
 *   - buffer: Pointer to a string containing the command pipeline.
 *   - job: Pointer to a Job structure where the parsed commands will be stored.
 *
 * Output:
 *   - job: The Job structure is updated with the parsed commands.
 *
 * Assumptions/Limitations:
 *   - Assumes the command string is well-formed (e.g., no empty commands).
 *   - Assumes the Job structure has enough space to store the parsed commands.
 */
void parse_commands(char *buffer, struct Job *job)
{
    char *commands[MAX_ARGS + 1];
    unsigned int num_tokens = 0;

    tokenize_string(buffer, commands, &num_tokens, '|');
    job->num_stages = num_tokens;

    for (int i = 0; i < num_tokens; i++)
    {
        tokenize_string(commands[i], job->pipeline[i].argv, &job->pipeline[i].argc, ' ');
    }
}

/*
 * setup_pipes
 *
 * Purpose: 
 *    Creates pipes for communication between stages in a command pipeline.
 *
 * Inputs:
 *   - pipefd: Array to store the file descriptors for the pipes.
 *   - num_stages: Number of stages in the command pipeline.
 *
 * Output:
 *   - pipefd: Contains the file descriptors for the created pipes.
 *
 * Assumptions/Limitations:
 *   - Assumes num_stages is greater than 1.
 *   - Assumes the system has enough resources to create pipes.
 */
void setup_pipes(int *pipefd, int num_stages)
{
    for (int i = 0; i < num_stages - 1; i++)
    {
        if (pipe(pipefd + 2 * i) == -1)
        {
            write(2, "pipe failed\n", 12);
            return;
        }
    }
}

/*
 * handle_redirections
 *
 * Purpose: 
 *    Sets up input/output redirections for a specific stage in the command pipeline.
 *
 * Inputs:
 *   - job: Pointer to a Job structure containing the command pipeline.
 *   - i: Index of the current stage in the pipeline.
 *   - pipefd: Array of file descriptors for the pipes.
 *
 * Assumptions/Limitations:
 *   - Assumes the Job structure has been properly populated.
 *   - Assumes the pipe file descriptors are valid.
 */
void handle_redirections(struct Job *job, int i, int *pipefd)
{
    if (i == 0 && job->infile_path)
    {
        int infile_fd = open(job->infile_path, O_RDONLY);
        if (infile_fd == -1)
        {
            write(2, "error: cannot open input file\n", 30);
            _exit(1);
        }
        dup2(infile_fd, STDIN_FILENO);
        close(infile_fd);
    }

    if (i == job->num_stages - 1 && job->outfile_path)
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
        if (i > 0) {
            dup2(pipefd[2 * (i - 1)], STDIN_FILENO);
        }
        if (i < job->num_stages - 1)
        {
            dup2(pipefd[2 * i + 1], STDOUT_FILENO);
        }
    }
}

/*
 * close_pipes
 *
 * Purpose: 
 *    Closes all file descriptors associated with the pipes in a command pipeline.
 *
 * Input:
 *   - pipefd: Array of file descriptors for the pipes.
 *   - num_stages: Number of stages in the command pipeline.
 *
 * Assumptions/Limitations:
 *   - Assumes the pipe file descriptors are valid.
 */
void close_pipes(int *pipefd, int num_stages)
{
    for (int i = 0; i < 2 * (num_stages - 1); i++)
    {
        close(pipefd[i]);
    }
}

/*
 * execute_command
 *
 * Purpose: 
 *    Executes a command in a specific stage of the pipeline.
 *
 * Inputs:
 *   - job: Pointer to a Job structure containing the command pipeline.
 *   - i: Index of the current stage in the pipeline.
 *
 * Assumptions/Limitations:
 *   - Assumes the command is valid and executable.
 *   - Assumes the Job structure has been properly populated.
 */
void execute_command(struct Job *job, int i)
{
    if (execve(job->pipeline[i].argv[0], job->pipeline[i].argv, NULL) == -1)
    {
        write(2, "error: command not found: ", 26);
        write(2, job->pipeline[i].argv[0], my_strlen(job->pipeline[i].argv[0]));
        write(2, "\n", 1);
        _exit(1);  
    }

    if (my_streq(job->pipeline[i].argv[0], "/bin/cat"))
    {
            write(2, "\n", 1);
    }
}

/*
 * get_valid_string
 *
 * Purpose: 
 *    Removes leading and trailing spaces from a string and ensures 
 *    it does not contain spaces between letters.
 *
 * Input:
 *   - str: Pointer to the string to be validated and cleaned.
 *
 * Output:
 *   - str: The string is modified in place to remove leading/trailing spaces.
 *
 * Assumptions/Limitations:
 *   - Assumes the string is null-terminated.
 *   - Does not handle strings with internal spaces (e.g., paths with spaces).
 */
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
                return;
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

    if (start != str)
    {
        i = 0;
        while (start[i] != '\0')
        {
            str[i] = start[i];
            i++;
        }
        str[i] = '\0';
    }
}

/*
 * handle_internal_command
 *
 * Purpose:
 *    Executes internal shell commands like "cd" and "exit".
 *
 * Input:
 *   - command: Pointer to a `Command` structure containing the command and its arguments.
 *
 * Behavior:
 *   - If no arguments are provided, the function returns without doing anything.
 *   - If the command is `"cd"`:
 *       - If no argument is given, it prints `"cd: missing argument"`.
 *       - If an argument is provided, it attempts to change the directory using `chdir()`.
 *       - If `chdir()` fails, it prints an error message using `perror()`.
 *   - If the command is `"exit"`:
 *       - The program terminates with `exit(0)`.
 *
 * Assumptions/Limitations:
 *   - Only supports `"cd"` and `"exit"`.
 *   - Does not handle advanced `cd` features like `cd -` (previous directory).
 *   - Relies on `my_streq()` for string comparison.
 */
void handle_internal_command(struct Command *command) {
    if (command->argc == 0) {
        return; // No command to execute
    }

    if (my_streq(command->argv[0], "cd")) {
        // Handle cd command
        if (command->argc < 2) {
            write(1, "cd: missing argument\n", 21);
        } else {
            if (chdir(command->argv[1]) != 0) {
                perror("cd");
            }
        }
    } else if (my_streq(command->argv[0], "exit")) {
        exit(0);
    }
}

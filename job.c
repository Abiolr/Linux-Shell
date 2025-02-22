#include "job.h"

void get_job(struct Job *job) {
    char *parse_infile[MAX_ARGS + 1];
    char *parse_outfile[MAX_ARGS + 1];
    unsigned int num_tokens;

    char buffer[MAX_COMMAND_LENGTH];
    int length;
    job->background = 0;

    my_free_all();
    read_input(buffer, &length);

    if (length > 0 && buffer[length - 1] == '&') {
        job->background = 1;
        buffer[length - 1] = '\0';
    }

    num_tokens = 0;
    tokenize_string(buffer, parse_infile, &num_tokens, '<');

    if (num_tokens == 2) {
        job->infile_path = parse_infile[1]; // The file path is the second token
        get_valid_string(job->infile_path);
        my_strncpy(buffer, parse_infile[0], my_strlen(parse_infile[0])); // Command is the first token
    } else if (num_tokens == 1) {
        if (parse_infile[0][0] == '<') {
            job->infile_path = parse_infile[0] + 1; // Skip the '<' character
            get_valid_string(job->infile_path);
            buffer[0] = '\0'; // No command
        } else {
            job->infile_path = NULL;
            my_strncpy(buffer, parse_infile[0], my_strlen(parse_infile[0]));
        }
    } else {
        write(2, "error: too many infile paths\n", 30);
        _exit(0);
    }

    tokenize_string(buffer, parse_outfile, &num_tokens, '>');
    if (num_tokens == 2) {
        job->outfile_path = parse_outfile[1];
        get_valid_string(job->outfile_path);
    } else if (num_tokens == 1) {
        job->outfile_path = NULL;
    } else {
        write(2, "error: too many outfile paths\n", 31);
        _exit(0);
    }
    my_strncpy(buffer, parse_outfile[0], my_strlen(parse_outfile[0]));
    
    parse_commands(buffer, job);
}

void run_job(struct Job *job) {
    int pipefd[2 * (job->num_stages - 1)];
    pid_t pids[job->num_stages];
    int child_status;

    if (job->num_stages > 1) setup_pipes(pipefd, job->num_stages);

    for (int i = 0; i < job->num_stages; i++) {
        pids[i] = fork();

        if (pids[i] == 0) {
            handle_redirections(job, i, pipefd);
            close_pipes(pipefd, job->num_stages);
            execute_command(job, i);
        } else if (pids[i] < 0) {
            write(2, "fork failed\n", 12);
            _exit(1);
        }
    }

    if (job->num_stages > 1) close_pipes(pipefd, job->num_stages);
    if (job->background == 0) {
        for (int i = 0; i < job->num_stages; i++) {
            waitpid(pids[i], &child_status, 0);
        }
    }
}

void read_input(char *buffer, int *length) {
    while (1) {
        write(2, "mysh$ ", 6);
        int bytes_read = read(0, buffer, MAX_COMMAND_LENGTH - 1);

        if (bytes_read == -1) {
            // If read was interrupted by a signal, restart the loop
            if (errno == EINTR) {
                continue;
            }
            perror("read");
            _exit(1);
        } else if (bytes_read == 0) {
            write(2, "\n", 1);
            _exit(0);
        }

        buffer[bytes_read] = '\0';

        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                *length = i;
                buffer[i] = '\0'; // Replace newline with null terminator
                return;
            }
        }

        // If no newline was found, set the length to the full buffer
        *length = bytes_read;
        buffer[bytes_read] = '\0';
        return;
    }
}

void parse_commands(char *buffer, struct Job *job) {
    char *commands[MAX_ARGS + 1];
    unsigned int num_tokens = 0;

    tokenize_string(buffer, commands, &num_tokens, '|');
    job->num_stages = num_tokens;

    for (int i = 0; i < num_tokens; i++) {
        tokenize_string(commands[i], job->pipeline[i].argv, &job->pipeline[i].argc, ' ');
    }
}

void setup_pipes(int *pipefd, int num_stages) {
    for (int i = 0; i < num_stages - 1; i++) {
        if (pipe(pipefd + 2 * i) == -1) {
            write(2, "pipe failed\n", 12);
            _exit(1);
        }
    }
}

void handle_redirections(struct Job *job, int i, int *pipefd) {
    if (i == 0 && job->infile_path) {
        int infile_fd = open(job->infile_path, O_RDONLY);
        if (infile_fd == -1) {
            write(2, "error: cannot open input file\n", 30);
            _exit(1);
        }
        dup2(infile_fd, STDIN_FILENO);
        close(infile_fd);
    }
    if (i == job->num_stages - 1 && job->outfile_path) {
        int outfile_fd = open(job->outfile_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outfile_fd == -1) {
            write(2, "error: cannot open output file\n", 31);
            _exit(1);
        }
        dup2(outfile_fd, STDOUT_FILENO);
        close(outfile_fd);
    }
    if (job->num_stages > 1) {
        if (i > 0) {
            dup2(pipefd[2 * (i - 1)], STDIN_FILENO);
        }
        if (i < job->num_stages - 1) {
            dup2(pipefd[2 * i + 1], STDOUT_FILENO);
        }
    }
}

void close_pipes(int *pipefd, int num_stages) {
    for (int i = 0; i < 2 * (num_stages - 1); i++) {
        close(pipefd[i]);
    }
}


void execute_command(struct Job *job, int i) {
    // Check if the command is an internal command
    if (my_streq(job->pipeline[i].argv[0], "cd") || my_streq(job->pipeline[i].argv[0], "exit")) {
        handle_internal_command(&job->pipeline[i]);
        _exit(0); // Exit the child process after handling the internal command
    }

    // If not an internal command, execute it using execve
    if (execve(job->pipeline[i].argv[0], job->pipeline[i].argv, NULL) == -1) {
        write(2, "error: command not found: ", 26);
        write(2, job->pipeline[i].argv[0], my_strlen(job->pipeline[i].argv[0]));
        write(2, "\n", 1);
        _exit(1);  
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

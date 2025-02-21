#include "constants.h"
#include "mylib.h"
#include "str_lib.h"
#include "struct.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef JOB_H
#define JOB_H

void get_job(struct Job *job);
void run_job(struct Job *job);
void read_input(char *buffer, int *length);
void parse_commands(char *buffer, struct Job *job);
void setup_pipes(int *pipefd, int num_stages);
void handle_redirections(struct Job *job, int i, int *pipefd);
void close_pipes(int *pipefd, int num_stages);
void execute_command(struct Job *job, int i);
void get_valid_string(char *str);

#endif

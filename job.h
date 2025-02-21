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
void get_valid_string(char *str);

#endif
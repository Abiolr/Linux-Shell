#include "constants.h"
#include "command.h"
#include "mylib.h"
#include "str_lib.h"

#ifndef JOB_H
#define JOB_H

struct Job
{
    struct Command pipeline[MAX_PIPELINE_LEN];
    unsigned int num_stages;     /* either 1 or 2 */
    char *outfile_path;          /* NULL for no output redirection     */
    char *infile_path;           /* NULL for no input redirection      */
    int background;              /* 0=foreground, otherwise=background */
};

void get_job(struct Job * job);
void run_job(struct Job * job);
void get_valid_string(char *str);

#endif
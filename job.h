#include "constants.h"
#include "command.h"

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

#endif
#include "constants.h"
#include "mylib.h"
#include "str_lib.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef STRUCT_H
#define STRUCT_H

struct Command
{
     char * argv[MAX_ARGS + 1];
     unsigned int argc;
     int background;              /* 0=foreground, otherwise=background */
};

struct Job
{
    struct Command pipeline[MAX_PIPELINE_LEN];
    unsigned int num_stages;     /* either 1 or 2 */
    char *outfile_path;          /* NULL for no output redirection     */
    char *infile_path;           /* NULL for no input redirection      */
    int background;              /* 0=foreground, otherwise=background */
};

#endif

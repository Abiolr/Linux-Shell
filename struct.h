/*
 * struct.h
 * 
 * This file defines the data structures used to represent commands and jobs.
 * The structures are designed to hold information about command-line arguments, input/output redirection,
 * and whether the command or job should run in the foreground or background.
 *
 * Structures:
 * - Command: Represents a single command with its arguments and execution mode (foreground/background).
 * - Job: Represents a pipeline of commands, including input/output redirection and execution mode.
 */

#include "constants.h"

#ifndef STRUCT_H
#define STRUCT_H

/*
 * Structure: Command
 * 
 * Represents a single command to be executed.
 * 
 * Fields:
 * - argv: An array of strings representing the command and its arguments.
 *         The last element is NULL to indicate the end of the arguments.
 * - argc: The number of arguments in the argv array (excluding the NULL terminator).
 * - background: Indicates whether the command should run in the background (non-zero) 
 *               or foreground (zero).
 */
struct Command
{
     char *argv[MAX_ARGS + 1];
     unsigned int argc;
};

/*
 * Structure: Job
 * 
 * Represents a job consisting of one or more commands in a pipeline, along with
 * input/output redirection and execution mode.
 * 
 * Fields:
 * - pipeline: An array of Command structures representing the pipeline stages.
 * - num_stages: The number of stages in the pipeline (either 1 or 2 in this implementation).
 * - outfile_path: Path to the file for output redirection. NULL if no output redirection is needed.
 * - infile_path: Path to the file for input redirection. NULL if no input redirection is needed.
 * - background: Indicates whether the job should run in the background (non-zero) 
 *               or foreground (zero).
 */
struct Job
{
    struct Command pipeline[MAX_PIPELINE_LEN];
    unsigned int num_stages;
    char *outfile_path;
    char *infile_path;
    int background;
};

#endif

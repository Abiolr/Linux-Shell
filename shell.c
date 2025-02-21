/*
 * shell.c
 * 
 * This program implements a simple shell program that reads commands from the user,
 * executes them, and continues to do so until the user enters the "exit" command.
 * 
 * The main loop of the shell reads a command, checks if it is the "exit" command,
 * and if not, executes the command using the run_command function. This process
 * repeats until the user inputs the "exit" command.
 */

#include "constants.h"
#include "str_lib.h"
#include "command.h"
#include "job.h"
#include <unistd.h>

/*
 * main
 * 
 * Purpose: 
 *   The main function of the shell program. It initializes the shell, reads commands
 *   from the user, and executes them until the user inputs the "exit" command.
 * 
 * Assumptions:
 *   - The get_command function correctly populates the Command structure.
 *   - The run_command function correctly executes the command.
 *   - The my_streq function correctly compares strings.
 * 
 * Limitations:
 *   - The "exit" command is the only way to terminate the program.
 */
int main()
{
    struct Job job;

    while (1) {
        // Get the job from user input
        get_job(&job);

        // Skip if no command was entered
        if (job.pipeline[0].argc == 0)
        {
            continue;
        }

        // Exit the shell if the user enters "exit"
        if (my_streq(job.pipeline[0].argv[0], "exit") == 1)
        {
            break;
        }

        // Run the job
        run_job(&job);
    }

    return 0;
}


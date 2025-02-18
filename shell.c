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

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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
int main() {
    struct Command command;
    
    get_command(&command);

    while (my_streq(command.argv[0], "exit") == 0)
    {
        run_command(&command);
        get_command(&command);
    }

    return 0;
}

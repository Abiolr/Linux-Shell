#include "signal_handle.h"

/*
Function name: handle_sigint
Inputs: int sig - Signal number
Outputs: None
Description: This function handles the SIGINT (Ctrl-C) signal by 
printing a new prompt and doing nothing else.
*/
void handle_sigint(int sig) {
    // Print a new prompt
    write(2, "\n", 1);
}

void signal_handler_setup(struct sigaction *sa) {
    sa->sa_handler = handle_sigint; // Set the signal handler
    sa->sa_flags = 0;               // No special flags
    sigemptyset(&sa->sa_mask);      // No signals are blocked during the handler

    // Register the signal handler for SIGINT
    if (sigaction(SIGINT, sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

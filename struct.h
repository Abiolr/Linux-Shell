#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_ARGS 10 

struct Command {
    char *argv[MAX_ARGS + 1]; 
    unsigned int argc;        
};

#endif

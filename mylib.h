#include "constants.h"

#include <stddef.h>

#ifndef MYLIB_H
#define MYLIB_H

char heap[HEAP_SIZE]; 
char *freep; 

char *my_alloc(unsigned int size); 
void my_free_all(void);

#endif

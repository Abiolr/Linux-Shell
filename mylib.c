#include <stddef.h>
#include "mylib.h"
#include "globals.h" 

char *my_alloc(unsigned int size) {
    if (freep + size > heap + HEAP_SIZE) {
        return NULL; 
    }
    char *ptr = freep; 
    freep += size;     
    return ptr;        
}

void my_free_all(void) {
    freep = heap; 
}

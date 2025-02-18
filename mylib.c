/*
 * mylib.c
 *
 * This file provides simple memory management functions for allocating and freeing memory
 * from a heap. It includes functions for allocating memory blocks and resetting
 * the heap to its initial state.
 *
 * The heap is managed using a global pointer that tracks the next available memory
 * location. The heap size is defined by `HEAP_SIZE` in the global configuration.
 */
#include <stddef.h>
#include "mylib.h"
#include "globals.h" 

/*
 * my_alloc
 *
 * Purpose:
 *   Allocates a block of memory of the specified size from the heap.
 *
 * Input:
 *   size - The number of bytes to allocate from the heap.
 *
 * Output:
 *   char* - A pointer to the allocated memory block, or NULL if there is insufficient space
 *           in the heap.
 *
 * Assumptions:
 *   - The heap is a fixed-size array (`heap`) defined in the global configuration.
 *   - The global pointer `freep` tracks the next available memory location in the heap.
 *   - The caller ensures that `size` is a positive value and does not exceed the available
 *     heap space.
 *
 * Limitations:
 *   - The function does not support freeing individual memory blocks; only a full heap reset
 *     is available via `my_free_all`.
 */
char *my_alloc(unsigned int size) {
    if (freep + size > heap + HEAP_SIZE) {
        return NULL; 
    }
    char *ptr = freep; 
    freep += size;     
    return ptr;        
}

/*
 * my_free_all
 *
 * Purpose:
 *   Resets the heap to its initial state by setting the free pointer (`freep`) back to the
 *   start of the heap. This effectively "frees" all previously allocated memory blocks.
 *
 * Input:
 *   None.
 *
 * Output:
 *   void - The heap is reset, and all memory is marked as available.
 *
 * Assumptions:
 *   - The heap is a fixed-size array (`heap`) defined in the global configuration.
 *   - The global pointer `freep` tracks the next available memory location in the heap.
 *
 * Limitations:
 *   - This function does not support freeing individual blocks of memory, it resets the entire heap.
 */
void my_free_all(void) {
    freep = heap; 
}

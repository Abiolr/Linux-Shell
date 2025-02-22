#include "mylib.h"
#include <stdio.h>

int main() {
    // Test my_alloc
    char *ptr1 = my_alloc(10);
    if (ptr1 != NULL)
    {
        printf("Allocated 10 bytes at %p\n", (void*)ptr1);
    }
    else
    {
        printf("Failed to allocate 10 bytes\n");
    }

    char *ptr2 = my_alloc(20);
    if (ptr2 != NULL) {
        printf("Allocated 20 bytes at %p\n", (void*)ptr2);
    }
    else
    {
        printf("Failed to allocate 20 bytes\n");
    }

    // Test my_free_all
    my_free_all();
    printf("Heap reset\n");

    // Allocate again after reset
    char *ptr3 = my_alloc(15);
    if (ptr3 != NULL)
    {
        printf("Allocated 15 bytes at %p after reset\n", (void*)ptr3);
    }
    else
    {
        printf("Failed to allocate 15 bytes after reset\n");
    }

    return 0;
}
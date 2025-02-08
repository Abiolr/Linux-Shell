#include <stdio.h>
#include <assert.h>
#include "globals.h"
#include "mylib.h"


void test_my_alloc() {
    char *ptr1 = my_alloc(100);
    assert(ptr1 != NULL);
    printf("Allocated 100 bytes: %p\n", (void*)ptr1);

    char *ptr2 = my_alloc(200);
    assert(ptr2 != NULL);
    printf("Allocated 200 bytes: %p\n", (void*)ptr2);
    
    assert(ptr2 == ptr1 + 100);

    char *ptr3 = my_alloc(HEAP_SIZE);
    assert(ptr3 == NULL);
    printf("Allocation failed as expected for size larger than heap\n");

    my_free_all();
    printf("Memory freed\n");

    char *ptr4 = my_alloc(50);
    assert(ptr4 != NULL);
    printf("Allocated 50 bytes after freeing: %p\n", (void*)ptr4);
}

int main() {
    test_my_alloc();
    my_free_all();
    return 0;
}

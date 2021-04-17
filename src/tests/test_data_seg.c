#include <stdio.h>
#include <string.h>
#include "../GarbageCollector.h"
#include <assert.h>
// #include "test-utils.h"

void* ptr1 = NULL;
void* ptr2 = NULL;
int main (int argc, char** argv) {
    gc_init();
    char* mesg = "Test for DATA SEGMENT Begins.\n";
    write(1, mesg, strlen(mesg));

    ptr1 = gc_malloc(sizeof(int));
    printf("The first ptr is: %p\n", ptr1);
    assert(get_sbrk_mem() == sizeof(int) + sizeof(metadata));

    ptr1 = 0; mark_and_sweep(); /* Make the heap address unreachable. */
    ptr2 = gc_malloc(sizeof(int));
    printf("After sweeping away ptr1 and malloc a new memory to ptr2: %p\n", ptr2);
    assert(get_sbrk_mem() == sizeof(int) + sizeof(metadata));
    gc_exit();
    return 0;
}
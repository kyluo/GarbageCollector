#include <stdio.h>
#include "GarbageCollector.h"
#include <assert.h>
#include <string.h>
void test_for_gc_stack_scan();

/* Test for unreachable heap references. 
   Make with "make test_heap_ref"
*/
int main() {
    printf("Hello, world!");
    // gc_init();
    //char* mesg = "TEST For Unreachable Stack References Begins\n";
    //write(1, "TEST For Unreachable Stack References Begins\n", strlen(mesg));
    // test_for_gc_stack_scan();
    // mark_and_sweep();
    // int* second_ptr = gc_malloc(sizeof(int));
    /* The second_int in test_for_gc_stack_scan should be swept away, so the total memeory allocation should still be 2 int.*/
    // assert(get_active_mem_include_metadata() == 2 * (sizeof(metadata) + sizeof(int))); 
    // gc_exit();
    // puts("All tests for stack scan PASSED.");
    return 1;
}
#include <stdio.h>
#include "GarbageCollector.h"
#include <assert.h>
#include <string.h>
void test_heap_reference();
// SIZE OF METADATA is 40
/* Test for unreachable heap references. 
   Make with "make test_heap_ref"
*/
int main() {
    gc_init();
    char* mesg = "TEST For Unreachable Heap References Begins\n";
    write(1, mesg, strlen(mesg));
    test_heap_reference();
    gc_exit();
    puts("All tests for heap scan PASSED.");
    return 1;
}

/* Tests for references that are stored on heap memory. */
void test_heap_reference() {
    /* heap_ptr stores a heap address  */
    unsigned long* heap_ptr = gc_malloc(sizeof(unsigned long));
    *(heap_ptr) = gc_malloc(sizeof(unsigned long)); 
    assert(get_active_mem_include_metadata() == (48 + 48));
    *heap_ptr = 0; heap_ptr = 0; /* Now the heap address that heap_ptr stored is lost, so did the line above */
    mark_and_sweep();
    unsigned long* heap_ptr2 = gc_malloc(sizeof(unsigned long));
    assert(get_active_mem_include_metadata() == 48 + 48);
}

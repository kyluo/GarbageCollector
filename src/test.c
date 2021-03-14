#include <stdio.h>
#include <string.h>
#include "GarbageCollector.h"
#include <assert.h>
#include "test-utils.h"
// SIZE OF METADATA is 40
void* third_int_ptr = NULL;
int test_for_gc_memory_allocation() {
    /* Malloc one int and free */
    int* first_test = gc_malloc(sizeof(int));
    assert(first_test); // checking for null
    *first_test = 8;
    assert(*first_test == 8);
    gc_free(first_test); // Manually free it

    /* Malloc another one and sweep, check the new address and  */
    int* second_test = gc_malloc(sizeof(int));
    // That the freed memory is recycled instead of new sbrk
    assert(second_test == first_test); 
    assert(get_active_mem_include_metadata() == sizeof(metadata)  + sizeof(int));
    mark_and_sweep();
    assert(get_active_mem_include_metadata() == sizeof(metadata)); // The remaining active memory should just be one metadata

    /* After mark and sweep call again, the address should be recycled again */
    int* third_int = gc_malloc(sizeof(int));
    assert(second_test == third_int);
    third_int_ptr = third_int;
    return 0;
}



int main (int argc, char** argv) {
    // I wonder if &argc gives the bottom of the stack.
    gc_init();
    test_for_gc_memory_allocation();
    mark_and_sweep();
    int *fourth_int = gc_malloc(sizeof(int));
    assert(fourth_int == third_int_ptr);
    puts("TESTING DONE");
    gc_exit();
    return 0;
}
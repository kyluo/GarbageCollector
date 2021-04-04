#include <stdio.h>
#include <string.h>
#include "GarbageCollector.h"
#include <assert.h>
#include "test-utils.h"

// SIZE OF METADATA is 40
//void* second_int_ptr = NULL;
void* heap_ref = NULL;
void* temp_fifth = NULL;
void* temp_fourth = NULL;
char* string  = "s";
//static  = mal

/* Tests for references that are stored on heap memory. */
int test_heap_reference() {
    /* heap_ptr stores a heap address  */
    unsigned long* heap_ptr = gc_malloc(sizeof(unsigned long));
    heap_ref = heap_ptr;
    *(heap_ptr) = gc_malloc(sizeof(int)); assert(get_sbrk_mem() == (48 + 44));
    heap_ptr = 0; /* Now the heap address that heap_ptr stored is lost, so did the line above */
    mark_and_sweep(); /* So far the mark and sweep doesn't deal with datasegment, e.i. static/global variables. */
    metadata* heap_ptr_head = (metadata*)get_head();
    //assert(heap_ptr_head->is_free);
    assert(get_active_mem_include_metadata() == 48 + 44);
    return 0;
}

int main (int argc, char** argv) {
    gc_init();

    test_heap_reference();
    // puts("Basic Test on Heap Reference PASSED.");

    // return 0;

    gc_exit();
    return 0;
}
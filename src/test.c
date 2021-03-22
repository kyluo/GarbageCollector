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
unsigned long test_for_gc_stack_scan() {

    /* Malloc one int and free */
    int* first_int = gc_malloc(sizeof(int));
    assert(gethead() == first_int);
    assert(first_int); // checking for null
    *first_int = 8;
    assert(*first_int == 8); assert(get_sbrk_mem() == 44);
    gc_free(first_int); // Manually free i


    /* Malloc another one and sweep, check the new address and  */
    int* second_int = gc_malloc(sizeof(int));
    /* That the freed memory is recycled instead of new sbrk */
    assert(second_int == first_int); 
    assert(get_active_mem_include_metadata() == sizeof(metadata) + sizeof(int)); assert(get_sbrk_mem() == 44);
    mark_and_sweep();
    /* If mark and sweep is called here it should not be freed since it's still reachable within the scope */
    assert(get_active_mem_include_metadata() == sizeof(metadata) + sizeof(int)); 
    
    /* Allocate more, this time it should allocate new space since "second_int" is not freed yet */
    int* third_int = gc_malloc(sizeof(int)); 
    assert(get_sbrk_mem() == 88); assert(gethead() == third_int);
    assert(get_active_mem_include_metadata() == 2 * (sizeof(metadata) + sizeof(int))); 
    third_int = 0; /* now the third int should no longer be reachable*/
    mark_and_sweep(); 

    int* should_equal_to_third_int = gc_malloc(sizeof(int));
    assert(get_sbrk_mem() == 88); assert(gethead() == should_equal_to_third_int);
    /* Allocated memory should not change since third_int is swept away */
    assert(get_active_mem_include_metadata() == 2 * (sizeof(metadata) + sizeof(int))); 
    void* stack_top = NULL;
    asm volatile ("movq %%rbp, %0" : "=r" (stack_top));
    printf("The should_equal_to_third_int  is: %p\n", should_equal_to_third_int);
    printf("The second_intis: %p\n", second_int);
    /* Returnning will make second_int unreachable, so it should be freed. */
    return (unsigned long)second_int;
}


int second_scan_stack_testing() {

    int fourth_int = gc_malloc(sizeof(int));
    assert(get_sbrk_mem() == 176);

    temp_fourth = fourth_int;
    int fifth_int = gc_malloc(sizeof(int));
    assert(get_sbrk_mem() == 220);
    temp_fifth = fifth_int;
    fourth_int = 0;
    fifth_int = 0;
    mark_and_sweep();
    // assert(gethead() == fifth_int);
    int sixth = gc_malloc(sizeof(int));
    assert(sixth == temp_fifth || sixth == temp_fourth);

    int seventh = gc_malloc(sizeof(int));
    assert(sixth == temp_fifth || sixth == temp_fourth);

    return 0;
}

/* Tests for references that are stored on heap memory. */
int test_heap_reference() {
    /* heap_ptr stores a heap address  */
    unsigned long* heap_ptr = gc_malloc(sizeof(unsigned long));
    heap_ref = heap_ptr;
    *(heap_ptr) = gc_malloc(sizeof(int)); assert(get_sbrk_mem() == (48 + 44));
    heap_ptr = 0; /* Now the heap address that heap_ptr stored is lost, so did the line above */
    mark_and_sweep(); /* So far the mark and sweep doesn't deal with datasegment, e.i. static/global variables. */
    metadata* heap_ptr_head = (metadata*)heap_ptr_head - 1;
    assert(heap_ptr_head->is_free);
    return 0;
}

int main (int argc, char** argv) {
    gc_init();


    int * second_int = test_for_gc_stack_scan();
    void* stack_top = NULL;
    asm volatile ("movq %%rbp, %0" : "=r" (stack_top));
    printf("The current stack address outside the test function is: %p\n", stack_top);

    metadata* itshead = ((metadata*)second_int - 1);
    assert(itshead->is_free == 0);
    
    mark_and_sweep();
    printf("The head address is: %p\n", gethead());
    assert(itshead->is_free == 0);
    /* After calling mark-and-sweep, all allocated memories in the test_for_gc_stack_scan function should be recycled. */
    int *third_int = gc_malloc(sizeof(int));
    assert(get_sbrk_mem() == 88);


    int *fourth = gc_malloc(sizeof(int));
    assert(get_sbrk_mem() == 132);
    /* Note that third malloc doesn't make further allocation, because the "second" is swept away. */
    //assert(get_active_mem_include_metadata() == sizeof(metadata) + sizeof(int)); /* The total mem should still be metadata + one int */
    second_scan_stack_testing();

    printf("active memory: %d \n", get_active_mem_include_metadata());
    puts("Stack-scan test DONE");

    // test_heap_reference();
    // puts("Basic Test on Heap Reference PASSED.");

    // return 0;
    
    gc_exit();
    return 0;
}
#include <stdio.h>
#include "../GarbageCollector.h"
#include <assert.h>
#include <string.h>
void test_for_gc_stack_scan();

/* Test for unreachable stack references. 
   Make with "make test_stack_ref"
*/
int main() {
    gc_init();
    char* mesg = "TEST For Unreachable Stack References Begins\n";
    write(1, "TEST For Unreachable Stack References Begins\n", strlen(mesg));
    test_for_gc_stack_scan();
    mark_and_sweep();
    int* second_ptr = gc_malloc(sizeof(int));
    int* third_ptr = gc_malloc(sizeof(int));
    printf("AFTER RETURNING: second_ptr: %p, third_ptr: %p\n", second_ptr,third_ptr);
    /* The second_int in test_for_gc_stack_scan should be swept away, so the total memeory allocation should still be 2 int.*/
    assert(get_sbrk_mem() == 3 * (sizeof(metadata) + sizeof(int))); 
    gc_exit();
    puts("All tests for stack scan PASSED.");
    return 1;
}

void test_for_gc_stack_scan() {

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
    printf("The third_int ptr is: %p\n", third_int);
    third_int = 0; /* now the third int should no longer be reachable*/
    mark_and_sweep(); 

    int* should_equal_to_third_int = gc_malloc(sizeof(int));
    printf("After freeing the third_int the newly allocated ptr is: %p\n", should_equal_to_third_int);
    assert(get_sbrk_mem() == 88); assert(gethead() == should_equal_to_third_int);
    /* Allocated memory should not change since third_int is swept away */
    assert(get_active_mem_include_metadata() == 2 * (sizeof(metadata) + sizeof(int))); 
    void* stack_top = NULL;
    asm volatile ("movq %%rbp, %0" : "=r" (stack_top));
    printf("The should_equal_to_third_int  is: %p\n", should_equal_to_third_int);
    printf("The second_int is: %p\n", second_int);


    /* Returnning will make second_int unreachable, so it should be freed. */
    /* So far, two integers + 2 metadata are allocated. */
    return;
}
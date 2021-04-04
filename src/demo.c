#include <stdio.h>
#include <string.h>
#include "GarbageCollector.h"
#include <assert.h>
#include "test-utils.h"


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
    printf("The total asked memory here is: %d,", get_sbrk_mem());
    assert(sixth == temp_fifth || sixth == temp_fourth);

    int seventh = gc_malloc(sizeof(int));
    assert(sixth == temp_fifth || sixth == temp_fourth);

    return 0;
}


/* A short usage demo */
int main(int argc, char** argv) {
    gc_init();
    int* a = gc_malloc(sizeof(int));

    gc_exit();
    return 0;
}
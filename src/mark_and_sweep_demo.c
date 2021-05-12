#include <stdio.h>
#include <string.h>
#include "GarbageCollector.h"
void my_func();
void my_func2();
void my_func3();
void final_demo();
int *global_int;

/* Mark-and-Sweep Demo Program */
int main(int argc, char **args) {
    gc_init();
    my_func(); /* After adding the stack ptr back to main, random_integer is no longer reachable.*/
    my_func2();
    // my_func3();
    gc_exit();
    return 0;
}

/* This function simply allocates an int heap memory.*/
void my_func() {
    int *random_integer = gc_malloc(sizeof(int));
    printf("LOG INSIDE my_func: The random_integer points to: %p\n", random_integer);
    mark_and_sweep(); /* Notice random_ineger is still reachable! The log should show it is reachable and marked. */
    printf("LOG INSIDE my_func: my_func ends. All heap memory allocated in this function is unreachable.\n");
}

void my_func2() {
    global_int = gc_malloc(sizeof(int)); /* Allocate heap space for this global variable. */
    printf("\nLOG INSIDE my_func2: The global_int points to: %p\n", global_int);
    /* Do something about it without worrying about freeing it, for example, setting it to 0. */
    global_int = 0;
    /* If you run mark_and_sweep here, the allocated heap memory will be freed, but I will let it do in gc_exit at the end. */
}

void my_func3() {
    char ** string = gc_malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        string[i] = gc_malloc(sizeof(char));
    }
}


// void final_demo() {
//     gc_init();
//     char ** string = gc_malloc(3 * sizeof(char*));
//     for (int i = 0; i < 3; i++) {
//         string[i] = gc_malloc(sizeof(char));
//         string[i] = 0; /* To make the memory above not reachable.*/
//     }
//     string = 0;
//     gc_exit();
//     exit(0);
// }
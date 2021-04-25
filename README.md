<!-- # GarbageCollector

Define a struct
```
typedef struct {
    size_t size;  // Number of bytes this piece of memory holds (excluding the metadata size)
    struct _meta_data *next;  // Pointer to the next instance of meta_data in the list
    int marked;  // The flag for sweep
    void (*dtor)(void*);  // Corresponding Destructor
} metadata; 
```
which store memory and related information that user request

Define a struct
```
typedef struct {
    size_t size;  // number of total byte
    void* top_stack;  // starting stack to loop through to check
    size_t num_items;  // number of allocated objects
    size_t marked_items;  //number of marked itmes
    metadata* head;  // the root of metadata
} GarbageCollector;
```
which hold the neccesary informations for garbage collector

goal
====
loop through stack from beginning ``top_stack`` untill end (last variable where we call end) to decide if there is unreachable memmory address in heap:
mark all rechable memory and sweep all unreachable ones

undecided
=========
if data is unreachable:<br/>
ignore the unreachable memory and malloc new memory as user request<br/>
or<br/>
check if there is memory block that can be used for future memory request<br/> -->
# Garbage Collect: mark & sweep and reference-counting garbage collection for C
## Project Members: Ruipeng Han, Kaiyuan Luo

This program is an implementation of a conservative, thread-local, mark-and-sweep
garbage collector. The implementation provides a fully functional replacement
for the standard POSIX `malloc()`, `calloc()`, `realloc()`, and `free()` calls.

## Quickstart

### Download, compile and test

    $ git clone https://github.com/kyluo/GarbageCollector.git
    $ cd src
    
To compile using the `clang` compiler:

    $ make test or make
    
To use the GNU Compiler Collection (GCC):

    $ make test CC=gcc
    
These tests should complete successfully. To create the current coverage report:

    $ ./test_stack_ref ./test_heap_ref ./test_data_seg ./test_P


### Basic usage (Mark & Sweep Version)

```c
...
#include "../GarbageCollector.h"
...


void some_fun() {
    ...
    int* my_array = gc_calloc(&gc, 1024, sizeof(int));
    for (size_t i=0; i<1024; ++i) {
        my_array[i] = 42;
    }
    ...
    // look ya, no free!
}

int main(int argc, char* argv[]) {
    gc_init();
    ...
    some_fun();
    ...
    gc_exit();
    return 0;
}
```

### Basic usage (Reference-Counting Version)
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

This program is an implementation of a conservative, thread-local, mark-and-sweep and referece-counting
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


### Data Structures

The core data structure inside of the Garbage Collecter is a struct that holds the metadata information of
allocated memory. 

```c
typedef struct metadata {
    // Number of bytes this piece of memory holds (excluding the metadata size)
    void* memory_ptr;
    // Number of bytes this piece of memory holds (excluding the metadata size)
    size_t size;
    // Pointer to the next instance of meta_data in the list
    struct metadata *next;
    // Pointer to the next instance of meta_data in the list
    struct metadata *prev;
    // The flag for sweep (1 means in use, 0 means free)
    int is_free;
    // The flag for sweep (1 means in use, 0 means free)
    int marked;
} metadata;
```

### The Mark-and-Sweep Algorithm

The naïve mark-and-sweep algorithm runs in two stages. First, in a *mark*
stage, the algorithm finds and marks all *root* allocations and all allocations
that are reachable from the roots.  Second, in the *sweep* stage, the algorithm
passes over all known allocations, collecting all allocations that were not
marked and are therefore deemed unreachable.

### The Reference-Counting Algorithm



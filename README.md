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
# Garbage Collector: Mark & Sweep and Reference-Counting garbage collection for C
## Project Members: Ruipeng Han, Kaiyuan Luo
## Project Mentor: David James

This program is an implementation of a conservative, thread-local, mark-and-sweep and referece-counting
garbage collector. The implementation provides a fully functional replacement
for the standard POSIX `malloc()`, `calloc()`, `realloc()`, and `free()` calls. 64-bit system.

## Quickstart
### Environment Requirment
This program can only run under a Linux environment (Ubuntu, Debian, etc) 64-bit system.
### Download, compile and test

    $ git clone https://github.com/kyluo/GarbageCollector.git
    $ cd src
    
To use the GNU Compiler Collection (GCC):

    $ make test or make
    
To compile using the `clang` compiler:

    $ make test CC=clang
    
These tests should complete successfully. To create the current coverage report:

    $ ./test_stack_ref ./test_heap_ref ./test_data_seg ./test_P


### Basic usage (Mark & Sweep Version)

```c
...
#include "../GarbageCollector.h"
...


void some_fun() {
    ...
    int* my_array = gc_calloc(1024, sizeof(int));
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
For reference-counting version, it utilize the pointer wrapper which user have to explicitly declear the special pointer.
In the reference-counting version, the special pointer will keep track of the memory block each pointer points to and 
the memory block will keep track of how many reference (pointer) that points to it. When there is no more pointer points to the memory block
the memory block will be automatically freed.

#### Basic usage (Reference-counting)
```c
#include "../GarbageCollector_P.h"
int main(...) {
    GarbageCollector* g = gc_start();
    // initialze the garbage collector
    reference* r1 = gc_new_ref(g);
    // create a new special pointer
    int* p1 = gc_malloc(g, r1, sizeof(int));
    // user can freely malloc new spaces
    gc_copy_ref(g, r1, r2);
    // copy the content from one pointer to another
    gc_swtich_ref(g, r2, r3);
    // swap the content between two pointer, similar to swap funciton
    gc_end(g);
    // automatically free any allocated memory 
}

```

### Usage Warning:
This garbage collector works as a complete replacement of the standard POSIX `malloc()`, `calloc()` calls, etc; making these calls will result in undefined behavior. Please only use the API that garbage collector has provided (`gc_malloc()`, `gc_calloc()`,etc) when allocating memories. 

## Limitations
For reference-counting, it could not auto detect and delete the stack stored special pointer and it needs to be manually deleted if the user wants to free more space.
But since the user might reuse the memory allocated pointer in different functions, such as buffer and temp, limitation in unable to free stack pointer after function
returns will be minimized.


<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [Mark-and-Sweep Algorithm](https://www.geeksforgeeks.org/mark-and-sweep-garbage-collection-algorithm/)
* [Writing a Simple Garbage Collector in C](https://maplant.com/gc.html#org8125f7d)

/**
 *  
 * A simple garbage collector for cs 296-41 project
 * MUST RUN ON AN LINUX ENVIRONMENT
 * 
 * */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/**
 * Represents a list of objects allocated by the user
 * Heap grows upward to higher memory address
 * */
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




/* User uses our version of malloc */
void *gc_malloc(size_t size);

/* GC's version of free, internal use only and user should not use it. */
void gc_free(void* ptr);

/* Merge two consecutive free blocks on meta_ptr, if possible */
void Merge_free_neighbor_memory(metadata *meta_ptr);

/* Helper function of the Merge_free_neighbor_memory and it specifically merges previous free block */
void Merge_prev(metadata *meta_ptr);

/* Our version of calloc, in users API */
void *gc_calloc(size_t num, size_t size);

/* Initializes the collector, user must call at the beginning in main. */
void gc_init();

/* Turn off the garabage collector and free any unreachable memories */
void gc_exit();

/* Scan a bound of memory address and mark if the value pointed by it is an address in the heap. */
void scan_and_mark_region(unsigned long *sp, unsigned long *end);

/* Scan through the heap memories the gc had allocated and mark if there is a reference to another heap memory. */
void scan_and_mark_heap_ref(void);

/* This functions scans the BSS, heap, and stack; then it frees any unreachable variable. */
void mark_and_sweep(void);

/* Getter for the total sbrk memory. */
int get_sbrk_mem();

/* Getter for the head. */
metadata* get_head();

/* Getter for the current active memory that had been allocated to the user. */
int get_active_mem_include_metadata();

/* Returns the bottom (Highest stack address) of the stack. */
void* get_stack_bottom();

/* Get the head's memory ptr. */
void* gethead();

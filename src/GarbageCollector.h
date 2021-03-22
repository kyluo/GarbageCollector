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
 * 
 * Represents a list of objects allocated by the user
 * Heap grows upward to higher memory address
 * 
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

typedef struct {
    size_t size; 
    void* stack_top;    // Tentative member, since the top (lowest sp address) will change across calls.
    void* stack_bottom; // Where the stack begins;
    size_t num_items;
    size_t marked_items;
    metadata* head;     // Points to the head of the heap memory allocated to user.
    int initialized;
} GarbageCollector;


/////
void *gc_malloc(size_t size);
void gc_free(void* ptr);
void Merge_free_neighbor_memory(metadata *meta_ptr);
void Merge_prev(metadata *meta_ptr);
void *gc_calloc(size_t num, size_t size);
void gc_init();
void gc_exit();
void scan_and_mark_region(unsigned long *sp, unsigned long *end);
//static void add_to_free_list(metadata *target);
void scan_and_mark_heap_ref(void);
void mark_and_sweep(void);
int get_sbrk_mem();
metadata* get_head();
int get_active_mem_include_metadata();
void* get_stack_bottom();
void* gethead();
/////
// void add_to_metadata_list(GarbageCollector* gc, metadata* new_metadata, size_t request_size);
// void *gc_malloc(GarbageCollector* gc, size_t request_size);
// void *gc_calloc(GarbageCollector* gc, size_t num_elements, size_t element_size);
// void *gc_realloc(GarbageCollector* gc, void *ptr, size_t request_size);
// void gc_free(GarbageCollector* gc, void* ptr);
// void gc_init(GarbageCollector* gc);

// void garbage_collect_start(GarbageCollector *gc, void *stack_initial);
// void mark_and_sweep(void);
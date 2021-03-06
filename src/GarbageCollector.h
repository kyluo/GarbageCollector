/**
 *  
 * A simple garbage collector for cs 296-41 project
 * 
 * */
#include <stdlib.h>
#include <stdio.h>
/**
 * 
 * Represents a list of objects allocated by the user
 * */
typedef struct {
    // Number of bytes this piece of memory holds (excluding the metadata size)
    size_t size;
    // Pointer to the next instance of meta_data in the list
    struct _meta_data *next;
    // The flag for sweep
    int marked;
    // Corresponding Destructor
    void (*dtor)(void*);
} metadata;

typedef struct {
    size_t size; 
    void* stack_top;
    void* stack_bottom;
    size_t num_items;
    size_t marked_items;
    metadata* head;
} GarbageCollector;


void add_to_metadata_list(GarbageCollector* gc, metadata* new_metadata, size_t request_size);
void *gc_malloc(GarbageCollector* gc, size_t request_size);
void *gc_calloc(GarbageCollector* gc, size_t num_elements, size_t element_size);
void *gc_realloc(GarbageCollector* gc, void *ptr, size_t request_size);
void gc_free(GarbageCollector* gc, void* ptr);
void gc_init(void);

void garbage_collect_start(GarbageCollector *gc, void *stack_initial);
void mark(GarbageCollector *gc);
void sweep(GarbageCollector *gc);
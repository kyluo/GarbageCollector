/**
 *  
 * A simple garbage collector for cs 296-41 project
 * 
 * */
#include <stdlib.h>
#include <stdio.h>
typedef struct {
    // Number of bytes this piece of memory holds (excluding the metadata size)
    size_t size;

    // Pointer to the next instance of meta_data in the list
    struct _meta_data *next;
} metadata;

typedef struct {
    size_t size; 
} GarbageCollector;

extern metadata* head;
extern size_t total_memory_used;
extern size_t total_memory_freed;


void add_to_metadata_list(metadata* new_metadata, size_t request_size);
void *mini_malloc(size_t request_size);
void *mini_calloc(size_t num_elements, size_t element_size);
void *mini_realloc(void *ptr, size_t request_size);
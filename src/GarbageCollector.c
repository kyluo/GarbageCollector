#include "GarbageCollector.h"
#include <stdio.h>
#include <string.h>

/* Initialization of global variables */
metadata* head = 0;
size_t total_memory_used = 0;
size_t total_memory_freed = 0;

void add_to_metadata_list(metadata* new_metadata, size_t request_size) {
    new_metadata->size = request_size;
    // total_memory_requested += request_size;
    /* If head is null, just assign it to the new meta data*/
    if (head == NULL) {
        head = new_metadata;
        head->next = NULL;
        return;
    }

    /* If not, then use a temp to make new meta data the new head and link the old head*/
    metadata* temp = head;
    head = new_metadata;
    new_metadata->next = temp;
}

void *mini_malloc(size_t request_size) {
    return NULL;
}

void *mini_calloc(size_t num_elements, size_t element_size) {
    return NULL;
}
                  
void *mini_realloc(void *ptr, size_t request_size) {
    return NULL;
}

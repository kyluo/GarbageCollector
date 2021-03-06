#include "GarbageCollector.h"
#include <stdio.h>
#include <string.h>
#define UNTAG(p) (((unsigned int) (p)) & 0xfffffffc)

void garbage_collect_start(GarbageCollector *gc, void *stack_initial) {
    
}

void add_to_metadata_list(GarbageCollector* gc, metadata* new_metadata, size_t request_size) {
    new_metadata->size = request_size;
    // total_memory_requested += request_size;
    /* If head is null, just assign it to the new meta data*/
    if (gc->head == NULL) {
        gc->head = new_metadata;
        gc->head->next = NULL;
        return;
    }

    /* If not, then use a temp to make new meta data the new head and link the old head*/
    metadata* temp = gc->head;
    gc->head = new_metadata;
    gc->num_items++;
    new_metadata->next = temp;
}

void *gc_malloc(GarbageCollector* gc, size_t size) {
    if (!size)
        return NULL;

    metadata* memory = malloc(size + sizeof(metadata));
    if (!memory) {
        return NULL;
    }
    add_to_metadata_list(gc, memory, size);
    return memory + 1;
}

void *gc_calloc(GarbageCollector* gc, size_t num_elements, size_t element_size) {
    size_t size = num_elements * element_size;
    void* memory = mini_malloc(gc, size);
    if (!memory) // Fail to allocate memory
        return NULL;

    memset(memory, 0, size);
    return memory;
}
                  
void *gc_realloc(GarbageCollector* gc, void *ptr, size_t request_size) {
    if (!ptr && !request_size) {
        return NULL;
    } else if (!ptr) {
        /* If the ptr to reallocate is null it's the same as calling mini_malloc */
        return mini_malloc(gc, request_size);
    } else if (!request_size) {
        /* treat this like a call to mini_free and return NULL */
        mini_free(gc, ptr);
        return NULL;
    }
    
    metadata* previos_metadata = NULL;
    metadata* current = ptr;
    metadata* next_metadata = NULL;
    while (current) {
        next_metadata = current->next;
        void* current_memory = (void*) current + sizeof(metadata);
        if (current_memory == ptr) { // Find the right piece of memory to reallocate
            if (!previos_metadata) {
                gc->head = next_metadata;
            } else {
                previos_metadata->next = next_metadata;
            }            

            // Then reallocate the current metadata.
            metadata* new_memory = realloc(current, sizeof(metadata) + request_size);
            if (!new_memory)
                return NULL;

            // Pass in the address of the new memory metadata.
            add_to_metadata_list(gc, new_memory, request_size);

            // Return the address right after the meta data.
            return new_memory + 1;
        }

        previos_metadata = current;
        current = next_metadata;
    }
    return NULL;
}

void gc_free(GarbageCollector* gc, void *ptr) {
    if (!ptr) {
        return;
    }
    metadata* current = gc->head;
    metadata* next_metadata;
    metadata* previous_metadata = NULL;
    while (current) {
        next_metadata = current->next;
        void* current_ptr = (void*) current + sizeof(metadata);
        /* finds the right block of memory to be freed*/
        if (ptr == current_ptr) {
            if (!previous_metadata) {
                gc->head = next_metadata;
            } else {
                previous_metadata->next = next_metadata;
            }
            free(current);
            return;
        }
        previous_metadata = current;
        current = next_metadata;
    }
}

void GC_init(GarbageCollector* gc) {
    static int initted;

    if (initted) // already initialized
        return;
    FILE *statfp;
    initted = 1;

    statfp = fopen("/proc/self/stat", "r");
    assert(statfp != NULL);
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld "
           "%*ld %*ld %*ld %*ld %*llu %*lu %*ld "
           "%*lu %*lu %*lu %lu", &gc->stack_bottom);
    fclose(statfp);
    
    // usedp = NULL;
    // base.next = freep = &base;
    // base.size = 0;
}

void garbage_collect_start(GarbageCollector *gc, void *stack_initial) {
    mark(gc);
    sweep(gc);
}

void mark(GarbageCollector *gc) {

}


void sweep(GarbageCollector *gc) {

}




#include "GarbageCollector.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <mach-o/getsect.h>
/*
 * The size of a pointer.
 */
#define PTRSIZE sizeof(char*)
/*
 * Support for windows c compiler is added by adding this macro.
 * Tested on: Microsoft (R) C/C++ Optimizing Compiler Version 19.24.28314 for x86
 */
#if defined(_MSC_VER)
#define __builtin_frame_address(x)  ((void)(x), _AddressOfReturnAddress())
#endif

#define UNTAG(p) (((uintptr_t) p) & 0xfffffffc) // was originally unsigned int

static metadata base;           /* Zero sized block to get us started. */
static metadata *freep = &base; /* Points to first free block of memory. */
static metadata *usedp;         /* Points to first used block of memory. */
static uintptr_t stack_bottom;

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
    void* memory = gc_malloc(gc, size);
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
        return gc_malloc(gc, request_size);
    } else if (!request_size) {
        /* treat this like a call to mini_free and return NULL */
        gc_free(gc, ptr);
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

void gc_init(GarbageCollector* gc) {

    if (gc->initialized)
        return;

    FILE *statfp;

    statfp = fopen("/proc/self/stat", "r");
    assert(statfp != NULL);
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld "
           "%*ld %*ld %*ld %*ld %*llu %*lu %*ld "
           "%*lu %*lu %*lu %lu", &stack_bottom);
    fclose(statfp);
    gc->stack_bottom = (void*) stack_bottom;
    usedp = NULL;
    base.next = freep = &base; // asign base_next and freep to address of base.
    base.size = 0;
    gc->initialized = 1;
}



//////////******


/*
 * Scan a region of memory and mark any items in the used list appropriately.
 * Both arguments should be word aligned.
 */
static void scan_region(unsigned long *sp, unsigned long *end) {
    metadata *bp;
    // for each addresses (each 8 byte), see if the value pointed by it matches one in the used list.
    for (; sp < end; sp++) {
        unsigned long v = *sp; // 取到sp指向的地址的值
        bp = usedp;
        do {
            // 如果那个值是一个地址 且跟used list中有吻合的地址，mark in the used list。
            if ((unsigned long)bp + 1 <= v && (unsigned long)(bp + 1 + bp->size) > v) {
                    bp->next = (metadata*) (((uintptr_t) bp->next) | 1);
                    break;
            }
        } while ((bp = (metadata*) UNTAG(bp->next)) != usedp);
    }
}


/*
 * Scan the free list and look for a place to put the block. Basically, we're 
 * looking for any block that the to-be-freed block might have been partitioned from.
 */
static void add_to_free_list(metadata *target) {
    metadata *curr_free_ptr;
    // Loop until bp > p && bp < p->next
    for (curr_free_ptr = freep; !(target > curr_free_ptr && target < curr_free_ptr->next); curr_free_ptr = curr_free_ptr->next)
        if (curr_free_ptr >= curr_free_ptr->next && (target > curr_free_ptr || target < curr_free_ptr->next))
            break;
    // If the size of the target matches the the current block's next, then merge them!
    if (target + target->size == curr_free_ptr->next) {
        target->size += curr_free_ptr->next->size;
        target->next = curr_free_ptr->next->next;
    } else
        target->next = curr_free_ptr->next;

    if (curr_free_ptr + curr_free_ptr->size == target) {
        curr_free_ptr->size += target->size;
        curr_free_ptr->next = target->next;
    } else
        curr_free_ptr->next = target;

    freep = curr_free_ptr;
}

/*
 * Scan the marked blocks for references to other unmarked blocks.
 */
static void scan_heap(void) {
    unsigned int *vp; // was orginall unsigned int*
    metadata *bp, *up;

    for (bp = (metadata*) UNTAG(usedp->next); bp != usedp; bp = (metadata*) UNTAG(bp->next)) {
        if (!((uintptr_t)bp->next & 1)) // if not marked, skip
            continue;
        for (vp = (unsigned int *)(bp + 1);
             vp < (unsigned int *)(bp + bp->size + 1);
             vp++) {
            unsigned int v = *vp;
            up = (metadata*) UNTAG(bp->next);
            do {
                if (up != bp &&
                    (unsigned int) up + 1 <= v &&
                    (unsigned int)(up + 1 + up->size) > v) {
                    up->next = (metadata*) (((uintptr_t) up->next) | 1);
                    break;
                }
            } while ((up = (metadata*) UNTAG(up->next)) != bp);
        }
    }
}

void mark_and_sweep(void) {
    metadata *p, *prevp, *tp;
    void* stack_top;
    extern char end, etext; /* Provided by the linker. */

    if (usedp == NULL)
        return;

    /* Scan the BSS and initialized data segments. */
    //scan_region(&etext, &end);
    scan_region((unsigned long*)get_etext(), (unsigned long*)get_end());


    /* Scan the stack. */
    //asm volatile ("movl %%ebp, %0" : "=r" (stack_top));
    stack_top = __builtin_frame_address(0);
    //stack_top = 0;
    scan_region((unsigned long*) stack_top, (unsigned long*) stack_bottom);

    /* Mark from the heap. */
    scan_heap();

    /* And now we sweep! */
    for (prevp = usedp, p = (metadata*) UNTAG(usedp->next);; prevp = p, p = (metadata*) UNTAG(p->next)) {
    next_chunk:
        if (!((unsigned int)p->next & 1)) {
            /*
             * The chunk hasn't been marked. Thus, it must be set free. 
             */
            tp = p;
            p = (metadata*) UNTAG(p->next);
            add_to_free_list(tp);

            if (usedp == tp) { 
                usedp = NULL;
                break;
            }

            prevp->next = (metadata*) ((uintptr_t)p | ((uintptr_t) prevp->next & 1));
            goto next_chunk;
        }
        p->next = (metadata*) (((unsigned long) p->next) & ~1);
        if (p == usedp)
            break;
    }
}
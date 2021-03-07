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

#define LAST_BIT_MASK(p) ((((uintptr_t) p)  >> 1) << 1) // make last bit 0.

static metadata base;           /* Zero sized block to get us started. */
static metadata *freep = &base; /* Points to first free block of memory. */
static metadata *usedp;         /* Points to first used block of memory. */
static uintptr_t stack_bottom;

static metadata* head = NULL;
static size_t total_sbrk_memory = 0;
static size_t current_active_memory = 0;

// Split the memory block into pieces that the user requested and the remaining piece,
// then make the linked sequence: ... -> remaining -> old -> ...
// Return 1 upon success and 0 upon failure
int splitBlock(size_t size, metadata *entry) {
    if (entry->size >= 2 * size && (entry->size-size) >= 1024) {

        metadata *new_entry = entry->memory_ptr + size; // Make a metdata for the new, free memory
        // update the new metadata's memory ptr, free, new size, and new next (the allocated memory)
        new_entry->memory_ptr = (new_entry + 1); 
        new_entry->is_free = 1;
        new_entry->size = entry->size - size - sizeof(metadata);
        new_entry->next = entry;
        if (entry->prev) {
            entry->prev->next = new_entry;
        } else {
            head = new_entry;
        }
        new_entry->prev = entry->prev;
        // Update the entry's new size and prev.
        entry->size = size;
        entry->prev = new_entry;
        return 1;
    }
    return 0;
}

/**
 *  Implementation of malloc using sbrk.
 * */
void *gc_malloc(size_t size) {
    // implement malloc!
    if (size == 0)
        return NULL;
    
    metadata* current_block = head;
    metadata* valid_block_memory = NULL;
    // if there are enough memory available:
    if (total_sbrk_memory - current_active_memory >= size) {
        while (current_block) {
            // If it's free and it fits:
            if (current_block->is_free && current_block->size >= size) {
                valid_block_memory = current_block;
                if (splitBlock(size, current_block)) { // Split the current block of memory based on user's requested size
                    current_active_memory += sizeof(metadata); // consider metadata when coalescing
                }
                break;
            }
            current_block = current_block->next;
        }
    }
    // If there is a suitable piece of memory
    if (valid_block_memory) {
        valid_block_memory->is_free = 0;
        current_active_memory += size;
    } 
    
    else {
        // if head exists and is free, allocate more on the head (since sbrk increases the address)
      if (head && head->is_free) {
        if (sbrk(size - head->size) == (void *)-1) // If sbrk failed
            return NULL;
        total_sbrk_memory += size - head->size;
        head->size = size;
        head->is_free = 0;
        valid_block_memory = head;
        current_active_memory += head->size;
      } else {
        valid_block_memory = sbrk(sizeof(metadata)+size);
        if (valid_block_memory == (void *)-1)
          return NULL;
        valid_block_memory->memory_ptr = valid_block_memory + 1;
        valid_block_memory->size = size;
        valid_block_memory->is_free = 0;
        valid_block_memory->next = head;
        if (head) {
          valid_block_memory->prev = head->prev;
          head->prev = valid_block_memory;
        } else {
          valid_block_memory->prev = NULL;
        }
        head = valid_block_memory; // update head.
        total_sbrk_memory += sizeof(metadata)+size;
        current_active_memory += sizeof(metadata)+size;
      }
    }

    return valid_block_memory->memory_ptr;
}

// The sequence (from high to low address): p next -> p -> p prev

void Merge_prev(metadata *meta_ptr) {
    meta_ptr->size += meta_ptr->prev->size + sizeof(metadata);
    meta_ptr->prev = meta_ptr->prev->prev;
    if (meta_ptr->prev) {
      meta_ptr->prev->next = meta_ptr;
    }
    else {
      head = meta_ptr;
    }
}

void Merge_free_neighbor_memory(metadata *meta_ptr) {
    // If previous is free, merge it.
    if (meta_ptr->prev && meta_ptr->prev->is_free == 1) {
        Merge_prev(meta_ptr);
        current_active_memory -= sizeof(metadata);// Remember to decrement the metadata size as well
    }
    // If next block is free, merge it.
    if (meta_ptr->next && meta_ptr->next->is_free == 1) {
        meta_ptr->next->size += meta_ptr->size + sizeof(metadata);
        meta_ptr->next->prev = meta_ptr->prev;
        if (meta_ptr->prev) {
            meta_ptr->prev->next = meta_ptr->next;
        } else {
            head = meta_ptr->next;
        }
        current_active_memory -= sizeof(metadata);
    }
}

void gc_free(void *ptr) {
    if (ptr == NULL) return;
    metadata *meta_ptr = (metadata*) ptr - 1;
    assert(meta_ptr->is_free == 0); // Check that it must not be freed first
    meta_ptr->is_free = 1;
    current_active_memory -= meta_ptr->size;
    Merge_free_neighbor_memory(meta_ptr); //
}

void *gc_calloc(size_t num, size_t size) {
    // Use my malloc and set the memory to 0.
    size_t memory_requested = num * size;
    void* memory = gc_malloc(memory_requested);
    if (memory != NULL)
        memset(memory, 0, memory_requested);
    return memory;
}

// Get the stack bot (highest $sp addresses)
void gc_init() {
    static int initialized;
    if (initialized)
        return;

    initialized = 1;
    FILE *statfp;

    statfp = fopen("/proc/self/stat", "r");
    assert(statfp != NULL);
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld "
           "%*ld %*ld %*ld %*ld %*llu %*lu %*ld "
           "%*lu %*lu %*lu %lu", &stack_bottom);
    fclose(statfp);
    usedp = NULL;
    base.next = freep = &base; // asign base_next and freep to address of base.
    base.size = 0;
}

//////////******
/*
 * Scan a region of memory and mark any items in the used list appropriately.
 * Both arguments should be word aligned.
 */
static void scan_and_mark_region(unsigned long *sp, unsigned long *end) {
    metadata *current_ptr = head;
    // for each addresses (each 8 byte), see if the value pointed by it matches one in the used list.
    for (; sp < end; sp++) {
        current_ptr = head;
        unsigned long stack_value = *sp; // 取到sp指向的地址的值
        while (current_ptr) {
            // If the stack value is an address between current_ptr's memory, mark it.
            if ((uintptr_t)(current_ptr + 1) <= stack_value && stack_value < (uintptr_t)(current_ptr + 1 + current_ptr->size)) {
                current_ptr->marked = 1;
                break;;
            }
            current_ptr = current_ptr->next;
        }
    }
}

/*
 * Scan the marked blocks for references to other unmarked blocks.
 */
static void scan_and_mark_heap_ref(void) {
    unsigned long *word; // was orginall unsigned int*
    metadata *current, *other_heap;

    for (current = head->next; current != NULL; current = current->next) {
        if (!current->marked) // if not marked, it's garbage, its content is not important. skip
            continue;
        // for each word in the current memory block (starting after metadata section), see if the value correspond to another
        // address in the heap, if it is, mark it.
        for (word = (unsigned long *)(current + 1);
             word < (unsigned long *)(current + current->size + 1);
             word++) {
            unsigned int address = *word; // get current word's value
            other_heap = current->next;
            do {
                // If address is in another heap mem block, mark it. We found a ref to other unmarked blocks.
                if (other_heap != current &&
                    (unsigned long) (other_heap + 1) <= address &&
                    address < (unsigned long)(other_heap + 1 + other_heap->size)) {
                    other_heap->marked = 1; // up->next = (metadata*) (((uintptr_t) up->next) | 1);
                    break;
                } ////TODO: make the linked list wrapped around at the end.
            } while ((other_heap = other_heap->next) != current); // Continue loop until hit current
        }
    }
}

void mark_and_sweep(void) {
    metadata *current_ptr, *prevp, *tp;
    void* stack_top;
    extern char end, etext; /* Provided by the linker. */

    if (usedp == NULL)
        return;

    /* Scan the BSS and initialized data segments. */
    //scan_region(&etext, &end);
    scan_and_mark_region((unsigned long*)get_etext(), (unsigned long*)get_end());


    /* Scan the stack. */
    //asm volatile ("movl %%ebp, %0" : "=r" (stack_top));
    stack_top = __builtin_frame_address(0);
    //stack_top = 0;
    scan_and_mark_region((unsigned long*) stack_top, (unsigned long*) stack_bottom);

    /* Mark from the heap. */
    scan_and_mark_heap_ref();

    for (current_ptr = head; current_ptr != NULL; current_ptr->next) {
        if (!current_ptr->marked) {
            gc_free(current_ptr+1);
            continue;
        }
        current_ptr->marked = 0;
    }
}
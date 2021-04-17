#include "GarbageCollector_P.h"
#include <stdio.h>
#include <string.h>
#include "includes/vector.h"

GarbageCollector* gc_start() {
    GarbageCollector* gc = malloc(sizeof(GarbageCollector));
    gc->_metadata_id = 1;
    gc->_reference_id = 1;
    gc->_references = shallow_vector_create();
    gc->_use = NULL;
    gc->_free = NULL;
    return gc;
}

void gc_end(GarbageCollector* gc) {
    metadata* curr = gc->_use;
    while (curr) {
        metadata* next = curr->_next;
        vector_destroy(curr->_refs);
        free(curr);
        curr = next;
    }

    curr = gc->_free;
        while (curr) {
        metadata* next = curr->_next;
        vector_destroy(curr->_refs);
        free(curr);
        curr = next;
    }

    for (size_t i = 0; i < vector_size(gc->_references); i++) {
        reference* temp = vector_get(gc->_references, i);
        free(temp);
    }
    vector_clear(gc->_references);
    vector_destroy(gc->_references);
    free(gc);
}

reference* gc_new_ref(GarbageCollector* gc) {
    reference* newRef = (reference*) malloc(sizeof(reference));
    newRef->_id = gc->_reference_id;
    newRef->_memory_id = 0;
    newRef->_addr = (void*) newRef;
    vector_push_back(gc->_references, newRef);

    gc->_reference_id += 1;
    return newRef;
}

void gc_copy_ref(GarbageCollector* gc, reference* oldRef, reference* newRef) {
    metadata* curr = gc->_use;
    while(curr) {
        if (curr->_id == oldRef->_memory_id) {
            vector_push_back(curr->_refs, newRef->_id);
            newRef->_memory_id = curr->_id;
            return;
        }
        curr = curr->_next;
    }
    return NULL;
}

void gc_swtich_ref(GarbageCollector* gc, reference* ref1, reference* ref2) {
    metadata* curr = gc->_use;
    metadata* data1 = NULL;
    metadata* data2 = NULL;
    while(curr) {
        if (curr->_id == ref1->_memory_id) {
            data1 = curr;
        }
        if (curr->_id == ref2->_memory_id) {
            data2 = curr;
        }
        curr = curr->_next;
    }
    // if no reference found
    if (!data1 || !data2) {
        return NULL;
    }
    // switch reference
    ref1->_memory_id = data2->_id;
    ref2->_memory_id = data1->_id;
    // switch refs form data
    for (size_t i = 0; i < vector_size(data1->_refs); i++) {
        if (vector_get(data1->_refs, i) == ref1->_id) {
            vector_erase(data1->_refs, i);
        }
    }
    for (size_t i = 0; i < vector_size(data2->_refs); i++) {
        if (vector_get(data2->_refs, i) == ref2->_id) {
            vector_erase(data2->_refs, i);
        }
    }
    vector_push_back(data1->_refs, ref2->_id);
    vector_push_back(data2->_refs, ref1->_id);
}

void* gc_deref(GarbageCollector* gc, reference* ref) {
    unsigned long metaData_id = ref->_memory_id;
    metadata* curr = gc->_use;
    while(curr) {
        if (curr->_id == metaData_id) { break; }
        curr = curr->_next;
    }
    if (!curr) { return NULL; }
    return curr + 1;
}

void gc_del_ref(GarbageCollector* gc, reference* ref) {

}

void* gc_malloc(GarbageCollector* gc, reference* ref, size_t request_size) {
    if (!ref) {
        ref = gc_new_ref(gc);
    }

    // check if any freed memory can be used
    if (gc->_free) {
        // fprintf(stderr, "has free memory\n");
        metadata* curr_free = gc->_free;
        metadata* curr_free_prev = gc->_free;
        while(curr_free) {
            // fprintf(stderr, "has free memory of request size: %d\n", curr_free->_size);
            if (curr_free->_size == request_size) {
                // assign memory block to the ref
                ref->_memory_id = curr_free->_id; 
                vector_push_back(curr_free->_refs, ref->_id);

                // change the memory block from free to used
                metadata* next = gc->_free->_next;
                    if (!gc->_use) {
                        gc->_use = curr_free;
                        curr_free->_next = NULL;
                    } else {
                        metadata* temp = gc->_use;
                        gc->_use = curr_free;
                        curr_free->_next = temp;
                    }  
                if (curr_free == gc->_free) {
                    gc->_free = next;
                } else {
                    curr_free_prev->_next = next;
                }
                //memset(curr_free + 1, 0, request_size);
                return curr_free + 1;
            }
            curr_free_prev = curr_free;
            curr_free = curr_free->_next;
        }
    }

    // fprintf(stderr, "no free found\n");
    
    // if no freed memory found, create new memory block to store new data
    metadata* newData = (metadata*) malloc(sizeof(metadata) + request_size);
    newData->_refs = shallow_vector_create();
    vector_push_back(newData->_refs, ref->_id);
    newData->_id = gc->_metadata_id;
    newData->_size = request_size;
    
    if (!gc->_use) {
        gc->_use = newData;
        newData->_next = NULL;
    } else {
        metadata* temp = gc->_use;
        gc->_use = newData;
        newData->_next = temp;
    }

    // remove ref from old metaData
    metadata* curr_use = gc->_use;
    while(curr_use) {
        // fprintf(stderr, "check remove reference: curr_id: %zu - ref_memory_id: %zu\n", curr_use->_id, ref->_memory_id);
        if (curr_use->_id == ref->_memory_id) {
            for (size_t i = 0; i < vector_size(curr_use->_refs); i++) {
                //fprintf(stderr, "%d - %d\n", vector_get(curr_use->_refs, i), ref->_id);
                if (vector_get(curr_use->_refs, i) == ref->_id) {
                    vector_erase(curr_use->_refs, i);
                    // fprintf(stderr, "removed\n");
                }
            }
            break; 
        }
        curr_use = curr_use->_next;
    }

    // set ref points to newData
    ref->_memory_id = newData->_id;

    gc->_metadata_id += 1;
    //memset(newData + 1, 0, request_size);
    return newData + 1;
}

void* gc_calloc(GarbageCollector* gc, reference* ref, size_t num_elements, size_t element_size) {
    return gc_malloc(gc, ref, num_elements * element_size);
}

void* gc_realloc(GarbageCollector* gc, reference* ref, size_t request_size) {
    if (!ref) {
        return gc_malloc(gc, ref, request_size);
    }
    if (request_size == 0) {
        gc_freeRef(gc, ref);
    }
    metadata* curr = gc->_use;
    while (curr) {
        if (ref->_memory_id = curr->_id) {
            break;
        }
    }
    curr = realloc(curr, sizeof(metadata) + request_size);
}

void gc_freeRef(GarbageCollector* gc, reference* ref) {
    metadata* curr_use = gc->_use;
    while(curr_use) {
        if (curr_use->_id == ref->_memory_id) {
            for (size_t i = 0; i < vector_size(curr_use->_refs); i++) {
                if (vector_get(curr_use->_refs, i) == ref->_id) {
                    vector_erase(curr_use->_refs, i);
                }
            }
            break; 
        }
        curr_use = curr_use->_next;
    }
    ref->_memory_id = 0;
}

void gc_clean(GarbageCollector* gc) {
    int reCheck = 0;
    metadata* curr = gc->_use;
    metadata* prev = gc->_use;
    while(curr) {
        // fprintf(stderr, "check curr used block: %d prev: %d\n", curr->_id, prev->_id);
        // for (size_t i = 0; i < vector_size(curr->_refs); i++) {
        //     fprintf(stderr, "%d -> ", vector_get(curr->_refs, i));
        // }
        // fprintf(stderr, "\n");
        if (vector_size(curr->_refs) == 0) {
            size_t start_addr = curr + 1;
            size_t end_addr = (void*)(curr + 1) + curr->_size;
            fprintf(stderr, "memory block starts at %p, ends at %p, value %d\n", start_addr, end_addr, *(size_t*)start_addr);
            for (size_t curr = start_addr; curr < end_addr; curr+=8) {
                size_t val = *(size_t*)curr;
                for (size_t i = 0; i < vector_size(gc->_references); i++) {
                    reference* curr = vector_get(gc->_references, i);
                    if (curr == val) {
                        gc_freeRef(gc, curr);
                    }
                }
            }
            // fprintf(stderr, "no reff\n");
            metadata* curr_next = curr->_next;
            metadata* head_free = gc->_free;
            gc->_free = curr;
            curr->_next = head_free;
            if (curr == prev) {
                gc->_use = curr_next;
            } else {
                prev->_next = curr_next;
                curr = curr_next;
            }
        } else {
            prev = curr;
            curr = curr->_next;
        }
    }
}
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "vector.h"

typedef struct {
    vector* _refs;
    unsigned long _id;
    size_t _size;
    struct metadata* _next;
} metadata;

typedef struct {
    unsigned long _id;
    unsigned long _memory_id;
} reference;

typedef struct {
    unsigned long _metadata_id;
    unsigned long _reference_id;
    vector* _references;
    metadata* _use;
    metadata* _free;
} GarbageCollector;

GarbageCollector* gc_start();
void gc_end(GarbageCollector* gc);

reference* gc_new_ref(GarbageCollector* gc);
void gc_copy_ref(GarbageCollector* gc, reference* oldRef, reference* newRef);
void gc_swtich_ref(GarbageCollector* gc, reference* ref1, reference* ref2);
void gc_del_ref(GarbageCollector* gc, reference* ref);

void* gc_malloc(GarbageCollector* gc, reference* ref, size_t request_size);
void* gc_calloc(GarbageCollector* gc, reference* ref, size_t num_elements, size_t element_size);
void* gc_deref(GarbageCollector* gc, reference* ref);

void gc_clean(GarbageCollector* gc);







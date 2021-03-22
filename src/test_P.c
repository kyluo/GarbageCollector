#include <stdio.h>
#include <string.h>
#include "GarbageCollector_P.h"

/*
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
*/

void print(GarbageCollector* g, reference* ref) {
    fprintf(stderr, "reference id: %zu, memory id: %zu, memory address: %p, value: %d\n", ref->_id, ref->_memory_id, gc_deref(g, ref), 
            gc_deref(g, ref) ? *(int*)gc_deref(g, ref) : 0);
}

int main () {
    GarbageCollector* g = gc_start();

    fprintf(stderr, "\033[0;32mtest malloc with same refference\033[0;37m\n");
    reference* r1 = gc_new_ref(g);
    print(g, r1);
    gc_malloc(g, r1, sizeof(int));
    int* p1 = gc_deref(g, r1);
    *p1 = 241;

    print(g, r1);
    
    gc_malloc(g, r1, sizeof(int));
    int* p2 = gc_deref(g, r1);
    *p2 = 296;

    print(g, r1);
    
    fprintf(stderr, "\033[0;32mtest clean will free first malloced blcok\033[0;37m\n");
    gc_clean(g);
    reference* r2 = gc_new_ref(g);
    print(g, r2);
    gc_malloc(g, r2, sizeof(int));
    print(g, r2);
    

    fprintf(stderr, "\033[0;32mtest copy reference\033[0;37m\n");
    reference* r3 = gc_new_ref(g);
    gc_copy_ref(g, r1, r3);
    print(g, r1); print(g, r2); print(g, r3);

    fprintf(stderr, "\033[0;32mtest switch reference\033[0;37m\n");
    gc_swtich_ref(g, r2, r3);
    print(g, r1); print(g, r2); print(g, r3);

    gc_end(g);

    return 0;
}
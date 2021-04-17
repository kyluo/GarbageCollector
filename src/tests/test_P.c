#include <stdio.h>
#include <string.h>
#include "../GarbageCollector_P.h"

void print(GarbageCollector* g, reference* ref) {
    fprintf(stderr, "reference id: %zu, reference address %p, memory id: %zu, memory address: %p, value: %d\n", ref->_id, ref->_addr, ref->_memory_id, gc_deref(g, ref), 
            gc_deref(g, ref) ? *(int*)gc_deref(g, ref) : 0);
}

int main () {
    GarbageCollector* g = gc_start();

    fprintf(stderr, "\033[0;32mtest malloc with same refference\033[0;37m\n");
    reference* r1 = gc_new_ref(g);
    print(g, r1);
    
    int* p1 = gc_malloc(g, r1, sizeof(int));
    *p1 = 241;
    print(g, r1);
    
    int* p2 = gc_malloc(g, r1, sizeof(int));
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
    gc_clean(g);

    fprintf(stderr, "\033[0;32mtest switch reference\033[0;37m\n");
    gc_swtich_ref(g, r2, r3);
    print(g, r1); print(g, r2); print(g, r3);
    gc_clean(g);

    fprintf(stderr, "\033[0;32mtest will detect reference in heap which its pointer is freed\033[0;37m\n");
    reference* r4 = gc_new_ref(g);  // Pointer to r4
    int* p3 = gc_malloc(g, r4, sizeof(int));
    *p3 = 374;

    reference** r5 = gc_malloc(g, r3, sizeof(reference*));
    *r5 = r4;
    fprintf(stderr, "%p\n", *(size_t*)gc_deref(g, r3));
    
    
    print(g, r1); print(g, r2); print(g, r3);print(g, r4);
    gc_clean(g);
    
    fprintf(stderr, "\033[0;31mshould grab previous int\033[0;37m\n");
    int* p4 = gc_malloc(g, r3, sizeof(int));
    print(g, r1); print(g, r2); print(g, r3);print(g, r4);
    gc_clean(g);

    fprintf(stderr, "\033[0;31mprevious reference in heap is deleted\033[0;37m\n");
    metadata* curr_free = g->_free;
    while (curr_free) {
        fprintf(stderr, "freed memory: %d\n", curr_free->_id);
        curr_free++;
    }
    metadata* curr_use = g->_use;
    while (curr_use) {
        fprintf(stderr, "used memory: %d\n", curr_use->_id);
        curr_use = curr_use->_next;
    }

    gc_end(g);

    return 0;
}
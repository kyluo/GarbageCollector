# GarbageCollector

Define a struct
``
typedef struct {
    // Number of bytes this piece of memory holds (excluding the metadata size)\
    size_t size;\
    // Pointer to the next instance of meta_data in the list\
    struct _meta_data *next; <br />
    // The flag for sweep <br />
    int marked; <br />
    // Corresponding Destructor <br />
    void (*dtor)(void*); <br />
} metadata; <br />
`` 
which store memory and related information that user request <br />

Define a struct <br />
``
typedef struct { <br />
    // number of total byte <br />
    size_t size;  <br />
    // starting stack to loop through to check <br />
    void* top_stack; <br />
    // number of allocated objects <br />
    size_t num_items; <br />
    //number of marked itmes <br />
    size_t marked_items; <br />
    // the root of metadata <br />
    metadata* head; <br />
} GarbageCollector; <br />
``

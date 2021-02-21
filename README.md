# GarbageCollector

Define a struct
``
typedef struct {
    // Number of bytes this piece of memory holds (excluding the metadata size)
    size_t size;  
    // Pointer to the next instance of meta_data in the list
    struct _meta_data *next;  
    // The flag for sweep
    int marked;  
    // Corresponding Destructor
    void (*dtor)(void*);  
} metadata;
`` 
which store memory and related information that user request

Define a struct
``
typedef struct {
    // number of total byte
    size_t size; 
    // starting stack to loop through to check
    void* top_stack;
    // number of allocated objects
    size_t num_items;
    //number of marked itmes
    size_t marked_items;
    // the root of metadata
    metadata* head;
} GarbageCollector;

``

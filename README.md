# GarbageCollector

Define a struct
```
typedef struct {
    size_t size;  // Number of bytes this piece of memory holds (excluding the metadata size)
    struct _meta_data *next;  // Pointer to the next instance of meta_data in the list
    int marked;  // The flag for sweep
    void (*dtor)(void*);  // Corresponding Destructor
} metadata; 
```
which store memory and related information that user request

Define a struct
```
typedef struct {
    size_t size;  // number of total byte
    void* top_stack;  // starting stack to loop through to check
    size_t num_items;  // number of allocated objects
    size_t marked_items;  //number of marked itmes
    metadata* head;  // the root of metadata
} GarbageCollector;
```
which hold the neccesary informations for garbage collector

#include <stdio.h>
#include "../GarbageCollector.h"

int main(void) {
    printf("Hello, world!\n");
    gc_init();
    printf("This line should not appear.");
    return 0;
}
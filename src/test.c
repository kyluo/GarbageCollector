#include <stdio.h>
#include <string.h>
#include "GarbageCollector.h"

int main (int argc, char** argv) {
    puts("Hello World.");
    // I wonder if &argc gives the bottom of the stack.
    
    gc_init();

    return 0;
}
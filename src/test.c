#include <stdio.h>
#include <string.h>
#include "GarbageCollector.h"
#include <assert.h>
int main (int argc, char** argv) {
    puts("Hello World.");
    // I wonder if &argc gives the bottom of the stack.
    FILE *statfp;
    //char path[80];
    statfp = fopen("/proc/self", "r");
    assert(statfp != NULL);
   // gc_init();

    return 0;
}
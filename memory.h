#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

void *getMemory(size_t size, int setToZero);

void *resizeMemory(void *memory, size_t size);

#endif

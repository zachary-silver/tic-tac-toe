#ifndef MEMORY_H
#define MEMORY_H

#define TRUE 1
#define FALSE 0

void* getMemory(size_t size, int setToZero, const char *errMessage);

void *resizeMemory(void *memory, size_t size, const char *errMessage);

#endif

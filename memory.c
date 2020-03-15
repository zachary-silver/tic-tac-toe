#include <stdlib.h>
#include <stdio.h>

#include "memory.h"

void* getMemory(size_t size, int setToZero, const char *errMessage) {
   void *memory;

   if ((memory = setToZero ? calloc(1, size) : malloc(size)) == NULL) {
      perror(errMessage);
      exit(EXIT_FAILURE);
   }

   return memory;
}

void *resizeMemory(void *memory, size_t size, const char *errMessage) {
   void *newMemory;

   if ((newMemory = realloc(memory, size)) == NULL && size != 0) {
      perror(errMessage);
      exit(EXIT_FAILURE);
   }

   return newMemory;
}

#include <stdlib.h>
#include <stdio.h>

#include "memory.h"

void *getMemory(size_t size, int setToZero) {
   void *memory;

   if ((memory = setToZero ? calloc(1, size) : malloc(size)) == NULL) {
      perror("getMemory");
      exit(EXIT_FAILURE);
   }

   return memory;
}

void *resizeMemory(void *memory, size_t size) {
   void *newMemory;

   if ((newMemory = realloc(memory, size)) == NULL && size != 0) {
      perror("resizeMemory");
      exit(EXIT_FAILURE);
   }

   return newMemory;
}

#ifndef orion_memory_h
#define orion_memory_h

#include <stddef.h>
#include <stdlib.h>

#define GROW_ARRAY(type, pointer, count) \
    (type*)reallocate(pointer, sizeof(type) * (count))

#define FREE_ARRAY(type, pointer)  reallocate(pointer, 0)

#define ALLOCATE(type, count) \
    (type*)reallocate(NULL, sizeof(type) * (count))

void* reallocate(void* pointer, size_t size);

#endif

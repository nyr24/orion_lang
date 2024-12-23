#ifndef orion_memory_h
#define orion_memory_h

#include <stddef.h>
#include <stdlib.h>

#define GROW_ARRAY(type, pointer, size)                                        \
    (type*)reallocate(pointer, sizeof(type) * (size))

#define FREE_ARRAY(type, pointer) reallocate(pointer, 0)

void* reallocate(void* pointer, size_t size);

#endif

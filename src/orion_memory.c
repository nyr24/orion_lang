#include "orion_memory.h"

void* reallocate(void* pointer, size_t size) {
    if (size == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, size);

    if (result == NULL) {
        free(pointer);
        exit(1);
    }

    return result;
}

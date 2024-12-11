#include "chunk.h"
#include "orion_memory.h"
#include "stdio.h"
#include <stdlib.h>

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = DEFAULT_CHUNK_CAPACITY;
    chunk->data = (uint8_t*)malloc(sizeof(uint8_t) * DEFAULT_CHUNK_CAPACITY);
    chunk->lines = (int*)malloc(sizeof(int) * DEFAULT_CHUNK_CAPACITY);
    initValueArr(&chunk->values);
}

void pushChunkEl(Chunk* chunk, uint8_t new_el, int line_number) {
    if (chunk->count == chunk->capacity) {
        uint32_t old_capacity = chunk->capacity;
        chunk->capacity *= 2;
        chunk->data = GROW_ARRAY(uint8_t, chunk->data, old_capacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, old_capacity, chunk->capacity);
    }

    chunk->data[chunk->count] = new_el;
    chunk->lines[chunk->count] = line_number;
    chunk->count++;
}

uint8_t popChunkEl(Chunk* chunk) {
    if (chunk->count == 0) {
        return -1;
    }

    int popped_el = chunk->data[chunk->count - 1];
    chunk->data[chunk->count - 1] = 0;
    chunk->lines[chunk->count - 1] = -1;
    chunk->count--;

    return popped_el;
}

void freeChunk(Chunk* chunk) {
    if (chunk->data != NULL) {
        free(chunk->data);
        free(chunk->lines);
        freeValueArr(&chunk->values);
        chunk->data = NULL;
        chunk->lines = NULL;
    }
}

uint8_t pushValueToChunk(Chunk* chunk, Value value) {
    pushValueArrEl(&chunk->values, value);
    return chunk->values.count - 1;
}

Value popValueFromChunk(Chunk* chunk) {
    Value popped_val = popValueArrEl(&chunk->values);
    return popped_val;
}

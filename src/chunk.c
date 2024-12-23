#include "chunk.h"
#include "orion_memory.h"
#include <stdio.h>
#include <stdlib.h>

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = DEFAULT_CHUNK_CAPACITY;
    chunk->data = (uint8_t*)malloc(sizeof(uint8_t) * DEFAULT_CHUNK_CAPACITY);
    chunk->lines = (int*)malloc(sizeof(int) * DEFAULT_CHUNK_CAPACITY);
    initValueArr(&chunk->constants);
}

void pushChunkEl(Chunk* chunk, uint8_t new_el, int* line_number,
                 bool should_inc_line) {
    if (chunk->count == chunk->capacity) {
        chunk->capacity *= 2;
        chunk->data = GROW_ARRAY(uint8_t, chunk->data, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, chunk->capacity);
    }

    chunk->data[chunk->count] = new_el;
    chunk->lines[chunk->count] = *line_number;
    chunk->count++;
    if (should_inc_line) {
        (*line_number)++;
    }
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
        freeValueArr(&chunk->constants);
        chunk->data = NULL;
        chunk->lines = NULL;
    }
}

void pushConstantToChunk(Chunk* chunk, Value constant, int* lineNumber) {
    pushValueArrEl(&chunk->constants, constant);

    uint8_t index = chunk->constants.count - 1;
    pushChunkEl(chunk, OP_CONSTANT, lineNumber, false);
    pushChunkEl(chunk, index, lineNumber, false);
    // (*lineNumber)++;
}

Value popConstantFromChunk(Chunk* chunk) {
    Value popped_val = popValueArrEl(&chunk->constants);
    return popped_val;
}

#include "chunk.h"
#include "stdio.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = DEFAULT_CHUNK_CAPACITY;
    chunk->data = (uint8_t*)malloc(sizeof(uint8_t) * DEFAULT_CHUNK_CAPACITY);
    initValueArr(&chunk->values);
}

void pushChunkEl(Chunk* chunk, uint8_t new_el) {
    if (chunk->count == chunk->capacity) {
        uint32_t old_capacity = chunk->capacity;
        chunk->capacity *= 2;
        chunk->data = GROW_ARRAY(uint8_t, chunk->data, old_capacity, chunk->capacity);
    }

    chunk->data[chunk->count] = new_el;
    chunk->count++;
}

uint8_t popChunkEl(Chunk* chunk) {
    if (chunk->count == 0) {
        return -1;
    }

    int popped_el = chunk->data[chunk->count - 1];
    chunk->data[chunk->count - 1] = 0;
    chunk->count--;

    return popped_el;
}

void disassembleInstruction(Chunk* chunk, uint32_t index) {
    OpCode instruction = chunk->data[index];

    switch (instruction) {
        case ADD:
            printf("ADD\n");
            break;
        case SUB:
            printf("SUB\n");
            break;
        case INC:
            printf("INC\n");
            break;
        case RET:
            printf("RET\n");
            break;
        default:
            printf("Unrecognized instruction %d at index: %u\n", instruction, index);
            break;
    }
}

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int i = 0; i < chunk->count; ++i) {
        disassembleInstruction(chunk, i);
    }
    printf("\n");
}

void freeChunk(Chunk* chunk) {
    if (chunk->data != NULL) {
        free(chunk->data);
        chunk->data = NULL;
        freeValueArr(&chunk->values);
    }
}

void pushValueToChunk(Chunk* chunk, Value value) {
    pushValueArrEl(&chunk->values, value);
}

Value pushValueFromChunk(Chunk* chunk) {
    Value popped_val = popValueArrEl(&chunk->values);
    return popped_val;
}
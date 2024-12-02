#ifndef orion_chunk_h
#define orion_chunk_h

#include <stdlib.h>
#include "orion_memory.h"
#include "common.h"

typedef enum {
    ADD,
    SUB,
    INC,
    RET
} OpCode;

typedef struct {
    uint32_t    count;
    uint32_t    capacity;
    uint8_t*    data;
} Chunk;

#define DEFAULT_CHUNK_CAPACITY 30

void initChunk(Chunk* chunk);

void pushChunkEl(Chunk* chunk, uint8_t new_el);

uint8_t popChunkEl(Chunk* chunk);

void printChunk(Chunk* chunk);

void freeChunk(Chunk* chunk);

#endif
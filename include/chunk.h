#ifndef orion_chunk_h
#define orion_chunk_h

#include <stdlib.h>
#include "orion_memory.h"
#include "common.h"
#include "value.h"

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_INC,
    OP_RET,
    OP_CONSTANT
} OpCode;

typedef struct {
    uint32_t    count;
    uint32_t    capacity;
    uint8_t*    data;
    ValueArr    values;
} Chunk;

#define DEFAULT_CHUNK_CAPACITY 30

void    initChunk(Chunk* chunk);
void    pushChunkEl(Chunk* chunk, uint8_t new_el);
uint8_t popChunkEl(Chunk* chunk);
void    freeChunk(Chunk* chunk);
void    disassembleChunk(Chunk* chunk, const char* name);
void    disassembleInstruction(Chunk* chunk, uint32_t offset);
void    pushValueToChunk(Chunk* chunk, Value value);
Value   pushValueFromChunk(Chunk* chunk);

#endif
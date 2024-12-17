#ifndef orion_chunk_h
#define orion_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_RET,
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NEGATE,
    OP_ADD,
    OP_SUB,
    OP_INC,
} OpCode;

typedef struct {
    ValueArr constants;
    int* lines;
    uint8_t* data;
    uint32_t count;
    uint32_t capacity;
} Chunk;

#define DEFAULT_CHUNK_CAPACITY 30

void initChunk(Chunk* chunk);
void pushChunkEl(Chunk* chunk, uint8_t new_el, int* line_number,
                 bool should_inc_line);
uint8_t popChunkEl(Chunk* chunk);
void freeChunk(Chunk* chunk);
void pushConstantToChunk(Chunk* chunk, Value constant, int* lineNumber);
Value popConstantFromChunk(Chunk* chunk);

#endif

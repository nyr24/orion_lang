#ifndef orion_chunk_h
#define orion_chunk_h

#include "common.h"
#include "constant.h"

typedef enum { OP_ADD, OP_SUB, OP_INC, OP_RET, OP_CONSTANT } OpCode;

typedef struct {
    ConstantArr constants;
    int* lines;
    uint8_t* data;
    uint32_t count;
    uint32_t capacity;
} Chunk;

#define DEFAULT_CHUNK_CAPACITY 30

void initChunk(Chunk* chunk);
void pushChunkEl(Chunk* chunk, uint8_t new_el, int line_number);
uint8_t popChunkEl(Chunk* chunk);
void freeChunk(Chunk* chunk);
uint8_t pushConstantToChunk(Chunk* chunk, Constant constant);
Constant popConstantFromChunk(Chunk* chunk);

#endif

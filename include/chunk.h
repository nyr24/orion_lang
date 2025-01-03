#ifndef orion_chunk_h
#define orion_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_RET,
    OP_NIL,
    OP_CONSTANT,
    // bool
    OP_TRUE,
    OP_FALSE,
    OP_NOT,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_GREATER_EQUAL,
    OP_LESS_EQUAL,
    // unary
    OP_NEGATE,
    OP_INC,
    OP_DEC,
    // binary
    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV
} OpCode;

typedef struct {
    ValueArr constants;
    int32_t* lines;
    uint8_t* data;
    int32_t count;
    int32_t capacity;
} Chunk;

#define DEFAULT_CHUNK_CAPACITY 30

void initChunk(Chunk* chunk);
void pushChunkEl(Chunk* chunk, uint8_t new_el, int* line_number,
                 bool should_increment_line);
uint8_t popChunkEl(Chunk* chunk);
void freeChunk(Chunk* chunk);
void pushConstantToChunk(Chunk* chunk, Value constant, int* lineNumber);
Value popConstantFromChunk(Chunk* chunk);

#endif

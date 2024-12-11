#include <stddef.h>
#include "chunk.h"
#include "debug.h"
#include <stdint.h>

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    int static line_number = 0;

    pushChunkEl(&chunk, OP_ADD, line_number++);
    pushChunkEl(&chunk, OP_SUB, line_number++);

    uint8_t valueIndex = pushValueToChunk(&chunk, 1.2);
    pushChunkEl(&chunk, OP_CONSTANT, line_number);
    pushChunkEl(&chunk, valueIndex, line_number++);

    pushChunkEl(&chunk, OP_RET, line_number++);
    
    disassembleChunk(&chunk, "my chunk");

    freeChunk(&chunk);
    
    return 0;
}

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

    // maybe should be a separate function
    uint8_t valueIndex = pushConstantToChunk(&chunk, 1.2);
    // 2 instructions should have the same line number
    pushChunkEl(&chunk, OP_CONSTANT, line_number);
    pushChunkEl(&chunk, valueIndex, line_number++);

    uint8_t valueIndex2 = pushConstantToChunk(&chunk, 2.8);
    pushChunkEl(&chunk, OP_CONSTANT, line_number);
    pushChunkEl(&chunk, valueIndex2 , line_number++);

    pushChunkEl(&chunk, OP_RET, line_number++);
    
    disassembleChunk(&chunk, "my chunk");

    freeChunk(&chunk);
    
    return 0;
}

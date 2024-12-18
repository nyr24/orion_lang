#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include <stddef.h>
#include <stdint.h>

int main(int argc, const char* argv[]) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    static int lineNumber = 123;

    pushConstantToChunk(&chunk, 5, &lineNumber);
    pushConstantToChunk(&chunk, 3, &lineNumber);
    pushChunkEl(&chunk, OP_MULT, &lineNumber, false);

    pushConstantToChunk(&chunk, 1, &lineNumber);
    pushConstantToChunk(&chunk, 5, &lineNumber);
    pushChunkEl(&chunk, OP_MULT, &lineNumber, false);

    pushChunkEl(&chunk, OP_DIV, &lineNumber, false);

    pushChunkEl(&chunk, OP_DEC, &lineNumber, false);
    pushChunkEl(&chunk, OP_INC, &lineNumber, false);
    pushChunkEl(&chunk, OP_NEGATE, &lineNumber, false);

    pushChunkEl(&chunk, OP_RET, &lineNumber, false);

    // disassembleChunk(&chunk, "my chunk");

    interpretChunk(&chunk);

    freeChunk(&chunk);
    freeVM();

    return 0;
}

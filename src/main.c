#include <stddef.h>
#include <stdio.h>
#include "chunk.h"
#include "value.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);
    pushChunkEl(&chunk, OP_ADD);
    pushChunkEl(&chunk, OP_INC);
    pushChunkEl(&chunk, OP_SUB);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);

    return 0;
}
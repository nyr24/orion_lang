#include <stddef.h>
#include <stdio.h>
#include "chunk.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);
    pushChunkEl(&chunk, ADD);
    pushChunkEl(&chunk, INC);
    pushChunkEl(&chunk, SUB);

    printChunk(&chunk);

    freeChunk(&chunk);

    return 0;
}
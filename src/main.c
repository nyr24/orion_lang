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

    pushConstantToChunk(&chunk, 3.2, &lineNumber);
    pushChunkEl(&chunk, OP_NEGATE, &lineNumber, false);
    pushChunkEl(&chunk, OP_RET, &lineNumber, false);

    disassembleChunk(&chunk, "my chunk");

    interpretChunk(&chunk);

    freeChunk(&chunk);
    freeVM();

    return 0;
}

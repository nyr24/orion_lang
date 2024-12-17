#include "chunk.h"
#include "debug.h"
#include <stdio.h>

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    uint8_t instruction = chunk->data[offset];

    switch (instruction) {
    case OP_RET:
        printf("OP_RET\n");
        return offset + 1;
    case OP_CONSTANT:
        printf("OP_CONSTANT; value: %lf; offset: %d\n",
               chunk->constants.data[chunk->data[offset + 1]],
               chunk->data[offset + 1]);
        // 2 byte instruction
        return offset + 2;
    case OP_CONSTANT_LONG:
        printf("OP_CONSTANT_LONG; value: %lf; offset: %d\n",
               chunk->constants.data[chunk->data[offset + 1]],
               chunk->data[offset + 1]);
        // 4 byte instruction
        return offset + 4;
    case OP_NEGATE:
        printf("OP_NEGATE\n");
        return offset + 1;
    case OP_ADD:
        printf("OP_ADD\n");
        return offset + 1;
    case OP_SUB:
        printf("OP_SUB\n");
        return offset + 1;
    case OP_INC:
        printf("OP_INC\n");
        return offset + 1;
    default:
        printf("Unrecognized instruction %d at offset: %d\n", instruction,
               offset);
        return offset + 1;
    }
}

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int i = 0; i < chunk->count;) {
        i = disassembleInstruction(chunk, i);
    }
    printf("\n");
}

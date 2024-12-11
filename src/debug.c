#include "debug.h"
#include <stdio.h>

void disassembleInstruction(Chunk* chunk, int* index) {
    OpCode instruction = chunk->data[*index];

    switch (instruction) {
        case OP_ADD:
            printf("OP_ADD\n");
            (*index)++;
            break;
        case OP_SUB:
            printf("OP_SUB\n");
            (*index)++;
            break;
        case OP_INC:
            printf("OP_INC\n");
            (*index)++;
            break;
        case OP_RET:
            printf("OP_RET\n");
            (*index)++;
            break;
        case OP_CONSTANT:
            printf("OP_CONSTANT; value: %lf; index: %d\n", 
                chunk->values.data[chunk->data[*index + 1]], chunk->data[*index + 1]);
            // 2 byte instruction
            (*index) += 2;
            break;
        default:
            printf("Unrecognized instruction %d at index: %d\n", instruction, *index);
            break;
    }
}

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int i = 0; i < chunk->count;) {
        // index incremented inside this function
        disassembleInstruction(chunk, &i);
    }
    printf("\n");
}

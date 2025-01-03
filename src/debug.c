#include "chunk.h"
#include "value.h"
#include "debug.h"
#include <stdio.h>

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    uint8_t instruction = chunk->data[offset];

    switch (instruction) {
    case OP_RET:
        return printSingleByteInstruction("OP_RET", offset);
    case OP_CONSTANT:
        return printConstantInstruction(chunk, "OP_CONSTANT", offset);
    case OP_TRUE:
        return printSingleByteInstruction("OP_TRUE", offset);
    case OP_FALSE:
        return printSingleByteInstruction("OP_FALSE", offset);
    case OP_NIL:
        return printSingleByteInstruction("OP_NIL", offset);
    case OP_NOT:
        return printSingleByteInstruction("OP_NOT", offset);
    case OP_EQUAL:
        return printSingleByteInstruction("OP_EQUAL", offset);
    case OP_GREATER_EQUAL:
        return printSingleByteInstruction("OP_GREATER_EQUAL", offset);
    case OP_GREATER:
        return printSingleByteInstruction("OP_GREATER", offset);
    case OP_LESS_EQUAL:
        return printSingleByteInstruction("OP_LESS_EQUAL", offset);
    case OP_LESS:
        return printSingleByteInstruction("OP_LESS", offset);
    case OP_NEGATE:
        return printSingleByteInstruction("OP_NEGATE", offset);
    case OP_INC:
        return printSingleByteInstruction("OP_INC", offset);
    case OP_DEC:
        return printSingleByteInstruction("OP_DEC", offset);
    case OP_ADD:
        return printSingleByteInstruction("OP_ADD", offset);
    case OP_SUB:
        return printSingleByteInstruction("OP_SUB", offset);
    case OP_MULT:
        return printSingleByteInstruction("OP_MULT", offset);
    case OP_DIV:
        return printSingleByteInstruction("OP_DIV", offset);
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

int printSingleByteInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int printConstantInstruction(Chunk* chunk, const char* name, int offset) {
    Value val = chunk->constants.data[chunk->data[offset + 1]];

    printf("%s; value: %lf; offset: %d\n", name, AS_NUMBER(val), chunk->data[offset + 1]);

    return offset + 2;
}

#ifndef orion_debug_h
#define orion_debug_h

#include "chunk.h"

int disassembleInstruction(Chunk* chunk, int index);
void disassembleChunk(Chunk* chunk, const char* name);
int printSingleByteInstruction(const char* name, int offset);
int printConstantInstruction(Chunk* chunk, const char* name, int offset,
                             bool is_long);

#endif

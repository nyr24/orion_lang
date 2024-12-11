#ifndef orion_debug_h
#define orion_debug_h

#include "chunk.h"

void disassembleInstruction(Chunk* chunk, int* index);
void disassembleChunk(Chunk* chunk, const char* name);

#endif

#ifndef orion_compiler_h
#define orion_compiler_h

#include "chunk.h"
#include "scanner.h"

typedef struct {
    Token prev;
    Token curr;
    bool hadError;
    bool panicMode;
} Parser;

bool compile(const char* source, Chunk* chunk);
void advanceParser();
void consume(TokenType tokenType, const char* message);
void errorAt(Token* token, const char* message);
void initParser();
void emitByte(uint8_t byte);
void emitBytes(uint8_t byte1, uint8_t byte2);
Chunk* getCurrentChunk();

#endif
